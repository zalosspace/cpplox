#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../utils/utils.h"

using std::string;
using std::vector;

static string trim(const string& s) {
    return utils::trim(s);
}

static vector<string> split(const string& s, char c) {
    return utils::split(s, c);
}

// ---------------- VISITOR ----------------
void defineVisitor(std::ofstream& file,
                   const string& baseName,
                   const vector<string>& types) {

    for (const string& type : types) {
        string typeName = trim(split(type, ':')[0]);
        file << "class " << typeName << ";\n";
    }

    file << "\nstruct Visitor {\n";
    for (const string& type : types) {
        string typeName = trim(split(type, ':')[0]);
        file << "    virtual Value visit" << typeName << baseName
             << "(const " << typeName << "& expr) = 0;\n";
    }
    file << "    virtual ~Visitor() = default;\n";
    file << "};\n\n";
}

// ---------------- TYPE ----------------
void defineType(std::ofstream& file,
                const string& baseName,
                const string& className,
                const string& fieldList) {

    file << "class " << className << " : public " << baseName << " {\n";
    file << "public:\n";

    vector<string> fields = split(fieldList, ',');

    // -------- Constructor --------
    file << "    " << className << "(";

    for (size_t i = 0; i < fields.size(); ++i) {
        string field = trim(fields[i]);
        string rawType = field.substr(0, field.find_last_of(' '));
        string name = field.substr(field.find_last_of(' ') + 1);

        string param = (name == "operator") ? "operator_" : name;
        string type = (rawType == "Expr*")
                        ? "std::unique_ptr<Expr>"
                        : rawType;

        file << type << " " << param;
        if (i < fields.size() - 1) file << ", ";
    }

    file << ")\n        : ";

    // -------- Initializer List --------
    for (size_t i = 0; i < fields.size(); ++i) {
        string field = trim(fields[i]);
        string rawType = field.substr(0, field.find_last_of(' '));
        string name = field.substr(field.find_last_of(' ') + 1);

        string param = (name == "operator") ? "operator_" : name;
        bool isExpr = (rawType == "Expr*");

        file << name << "(";
        if (isExpr)
            file << "std::move(" << param << ")";
        else
            file << param;
        file << ")";

        if (i < fields.size() - 1) file << ", ";
    }

    file << " {}\n\n";

    // -------- Fields --------
    for (string& field : fields) {
        field = trim(field);
        string rawType = field.substr(0, field.find_last_of(' '));
        string name = field.substr(field.find_last_of(' ') + 1);

        string type = (rawType == "Expr*")
                        ? "std::unique_ptr<Expr>"
                        : rawType;

        file << "    " << type << " " << name << ";\n";
    }

    // -------- Accept --------
    file << "\n    Value accept(Visitor& visitor) const override {\n";
    file << "        return visitor.visit" << className << baseName << "(*this);\n";
    file << "    }\n";

    file << "};\n\n";
}

// ---------------- AST FILE ----------------
void defineAst(const string& outputDir,
               const string& baseName,
               const vector<string>& types) {

    string path = outputDir + "/" + baseName + ".h";
    std::ofstream file(path);

    file << "#pragma once\n\n";
    file << "#include \"../src/Token/Token.h\"\n";
    file << "#include <variant>\n";
    file << "#include <memory>\n";
    file << "#include <string>\n\n";

    file << "using Value = std::variant<std::monostate, double, std::string, bool>;\n\n";

    defineVisitor(file, baseName, types);

    file << "class " << baseName << " {\n";
    file << "public:\n";
    file << "    virtual ~" << baseName << "() = default;\n";
    file << "    virtual Value accept(Visitor& visitor) const = 0;\n";
    file << "};\n\n";

    for (const string& type : types) {
        vector<string> parts = split(type, ':');
        string className = trim(parts[0]);
        string fields = trim(parts[1]);
        defineType(file, baseName, className, fields);
    }

    file.close();
}

// ---------------- MAIN ----------------
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: generate_ast <output directory>\n";
        return 64;
    }

    string outputDir = argv[1];

    // vector<string> types = {
    //     "Binary : Expr* left, Token operator, Expr* right",
    //     "Grouping : Expr* expression",
    //     "Literal : Value value",
    //     "Unary : Token operator, Expr* right",
    //     "Variable : Token name",
    // };

    vector<string> types = {
        "Expression : Expr* expression",
        "Print : Expr* expression",
        "Var : Token name, Expr initializer"
    };

    defineAst(outputDir, "Stmt", types);

    return 0;
}

