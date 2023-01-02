#ifndef __PARSER_H__
#define __PARSER_H__
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include "lexer.h"
using namespace std;

class Scope
{

public:
    typedef enum
    {
        STMT = 0,
        SCOPE
    } StmtType;

    struct stmt
    {
        StmtType type;

        std::string LHS;
        std::string RHS;
        Scope *scope;
    };

    std::vector<std::string> public_vars;
    std::vector<std::string> private_vars;
    std::vector<std::string> global_vars;
    std::vector<stmt *> stmts;

    int publicVarsLength;
    int privateVarsLength;
    Scope *parent = NULL;
    bool main = false;
    std::string name;

    // Default Constructor
    Scope()
    {
    }
};

class Parser
{

public:
    // class Scope
    // {

    // public:
    //     typedef enum
    //     {
    //         STMT = 0,
    //         SCOPE
    //     } StmtType;

    //     struct stmt
    //     {
    //         StmtType type;

    //         std::string LHS;
    //         std::string RHS;
    //         Scope *scope;
    //     };

    //     std::vector<std::string> public_vars;
    //     std::vector<std::string> private_vars;
    //     std::vector<std::string> global_vars;
    //     std::vector<stmt *> stmts;

    //     int publicVarsLength;
    //     int privateVarsLength;
    //     Scope *parent = NULL;
    //     bool main = false;
    //     std::string name;

    //     // Default Constructor
    //     Scope() {
    //     }
    // };

    // member functions
    void PrintInfo(Scope *pScope);
    void ParseProgram();
    void ParseGlobalVars();
    void PrintVec(std::vector<std::string> vec);
    void ConnectScopes(Scope *pScope);
    void ExecuteProgram(Scope *pScope);

    Scope::stmt *ParseStmt();

    std::string FindScope(std::string str, Scope *scope);

    Scope *ParseScope();
    void print1(Scope *scope);
    void print2(Scope *scope);
    std::vector<std::string> ParsePublicVars();
    std::vector<std::string> ParsePrivateVars();
    std::vector<Scope::stmt *> ParseStmtList();
    std::vector<std::string> ParseVarList();

    Token expect(TokenType tok);
    LexicalAnalyzer lexer;
    std::vector<std::string> globalVars;
    std::vector<Scope *> scopes;
    Scope *pmain;

    Parser()
    {
    }
};

#endif