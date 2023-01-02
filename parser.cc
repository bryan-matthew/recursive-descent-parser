#include "lexer.h"
#include"parser.h"
using namespace std;

void syntax_error()
{
    cout << "Syntax Error\n";
    exit(1);
}

void Parser::ParseProgram()
{

    ParseGlobalVars();
    pmain = ParseScope();
    pmain->global_vars = globalVars;
    pmain->main = true;

    pmain->parent = NULL;
}

void Parser::ParseGlobalVars()
{
    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();

    switch (t1.token_type)
    {
    case ID:
        if (t2.token_type == LBRACE)
        {
            lexer.UngetToken(t2);
            lexer.UngetToken(t1);
            return;
        }
        else if (t2.token_type == SEMICOLON || t2.token_type == COMMA)
        {
            lexer.UngetToken(t2);
            lexer.UngetToken(t1);
            globalVars = ParseVarList();
            expect(SEMICOLON);
        }
        break;
    default:
        syntax_error();
    }
}

void Parser::PrintVec(vector<string> vec)
{
    int i = 0;
    while (i < vec.size())
    {
        cout << vec[i] << endl;
        i++;
    }
}

vector<string> Parser::ParseVarList()
{
    vector<string> vars;
    vector<string> temp;
    vars.push_back(expect(ID).lexeme);

    Token t1 = lexer.GetToken();
    if (t1.token_type == SEMICOLON)
    {
        lexer.UngetToken(t1);
        return vars;
    }

    else if (t1.token_type == COMMA)
    {
        temp = ParseVarList();

        for (int i = 0; i < temp.size(); i++)
        {
            vars.push_back(temp[i]);
        }
        return vars;
    }

    else
    {
        syntax_error();
    }
}

void Parser::print1(Scope *scope)
{
    Token t = expect(ID);
    scope->name = t.lexeme;
    expect(LBRACE);
    scope->public_vars = ParsePublicVars();
    scope->private_vars = ParsePrivateVars();
}

void Parser::print2(Scope *scope)
{
    scope->privateVarsLength = scope->private_vars.size();
    scope->publicVarsLength = scope->public_vars.size();
    scope->stmts = ParseStmtList();
}

Scope *Parser::ParseScope()
{
    Scope *scope = new Scope;

    print1(scope);
    print2(scope);
    expect(RBRACE);

    return scope;
}

vector<string> Parser::ParsePublicVars()
{
    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();

    vector<string> variable1;

    switch (t1.token_type)
    {
    case PRIVATE:
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
        return variable1;
        break;
    case PUBLIC:
        if (t2.token_type == COLON)
        {
            variable1 = ParseVarList();
            expect(SEMICOLON);
            return variable1;
        }
        break;
    case ID:
        if (t2.token_type == EQUAL || t2.token_type == LBRACE)
        {
            lexer.UngetToken(t2);
            lexer.UngetToken(t1);
            return variable1;
        }
        break;
    default:
        syntax_error();
    }
}

vector<string> Parser::ParsePrivateVars()
{

    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();
    vector<string> variable1;

    switch (t1.token_type)
    {
    case PRIVATE:
        if (t2.token_type == COLON)
        {
            variable1 = ParseVarList();
            expect(SEMICOLON);
            return variable1;
        }
        break;
    case PUBLIC:
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
        return variable1;
        break;
    case ID:
        if (t2.token_type == EQUAL || t2.token_type == LBRACE)
        {
            lexer.UngetToken(t2);
            lexer.UngetToken(t1);
            return variable1;
        }
        break;
    default:
        syntax_error();
    }
}

vector<Scope::stmt *> Parser::ParseStmtList()
{

    vector<Scope::stmt *> stmts;
    vector<Scope::stmt *> temp;
    Scope::stmt *st = ParseStmt();
    stmts.push_back(st);
    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();

    if (t1.token_type == ID && (t2.token_type == EQUAL || t2.token_type == LBRACE))
    {
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
        temp = ParseStmtList();
        int i = 0;
        while (i < temp.size())
        {
            stmts.push_back(temp[i]);
            i++;
        }

        return stmts;
    }
    else
    {
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);

        return stmts;
    }
}

Scope::stmt *Parser::ParseStmt()
{
    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();
    Scope *scope = new Scope;
    Scope::stmt *st = new Scope::stmt;
    switch (t1.token_type)
    {
    case ID:
        if (t2.token_type == LBRACE)
        {
            st->type = Scope::SCOPE;
            lexer.UngetToken(t2);
            lexer.UngetToken(t1);
            st->scope = ParseScope();
            return st;
        }
        else if (t2.token_type == EQUAL)
        {
            st->type = Scope::STMT;
            t2 = expect(ID);
            expect(SEMICOLON);
            st->LHS = t1.lexeme;
            st->RHS = t2.lexeme;
            delete scope;
            return st;
        }
        break;
    default:
        syntax_error();
    }
}

Token Parser::expect(TokenType token_type)
{
    Token t = lexer.GetToken();

    if (t.token_type != token_type)
    {

        syntax_error();
    }
    return t;
}


std::string Parser::FindScope(std::string str, Scope *scope)
{

    string scoName = "";

    if (scope->main)
    {
        int i = 0;
        while(i < scope->public_vars.size()) {
            if (scope->public_vars[i] == str)
            {
                scoName = scope->name + '.';
                return scoName + str;
            }
            i++;
        }
        
        int j = 0;
        while(j < scope->private_vars.size()) {
            if (scope->private_vars[j] == str)
            {
                scoName = scope->name + '.';
                return scoName + str;
            }
            j++;
        }
        
        int k = 0;
        while(k < scope->global_vars.size()) {
            if (scope->global_vars[k] == str)
            {
                return "::" + str;
            }
            k++;
        }
        
        return "?." + str;
    }
    else
    {
        int i = 0, j = 0, k = 0;
        while(i < scope->public_vars.size()) {
             if (scope->public_vars[i] == str)
            {
                scoName = scope->name + '.';
                return scoName + str;
            }
            i++;
        }
        
        while(j < scope->private_vars.size()) {
             if (scope->private_vars[j] == str)
            {
                scoName = scope->name + '.';
                return scoName + str;
            }
            j++;
        }

        while(k < scope->private_vars.size()) {
             if (scope->private_vars[k] == str)
            {
                scoName = scope->name + '.';
                return scoName + str;
            }
            k++;
        }

        
        scope = scope->parent;
        while (!scope->main)
        {
            for (int i = 0; i < scope->public_vars.size(); i++)
            {
                if (scope->public_vars[i] == str)
                {
                    scoName = scope->name + '.';
                    return scoName + str;
                }
            }

            scope = scope->parent;
        }

        for (int i = 0; i < scope->public_vars.size(); i++)
        {
            if (scope->public_vars[i] == str)
            {
                scoName = scope->name + '.';
                return scoName + str;
            }
        }
        for (int i = 0; i < scope->global_vars.size(); i++)
        {
            if (scope->global_vars[i] == str)
            {
                return "::" + str;
            }
        }
        return "?." + str;
    }
}

void Parser::ExecuteProgram(Scope *pScope)
{
    int i = 0;

    while (i < pScope->stmts.size())
    {
        if (pScope->stmts[i]->type == Scope::STMT)
        {
            string strRHS = FindScope(pScope->stmts[i]->RHS, pScope);
            string strLHS = FindScope(pScope->stmts[i]->LHS, pScope);

            cout << strLHS << " = " << strRHS << endl;
        }
        else
        {
            ExecuteProgram(pScope->stmts[i]->scope);
        }
        i++;
    }
}

int main()
{
    Parser parser;
    Token token;

    parser.ParseProgram();

    parser.ConnectScopes(parser.pmain);

    parser.ExecuteProgram(parser.pmain);
    return 0;
}

void Parser::ConnectScopes(Scope *pScope)
{

    int i = 0;

    while (i < pScope->stmts.size())
    {
        if (pScope->stmts[i]->type == Scope::SCOPE)
        {
            pScope->stmts[i]->scope->parent = pScope;
            ConnectScopes(pScope->stmts[i]->scope);
        }
        i++;
    }
}

void Parser::PrintInfo(Scope *pScope)
{
    int i = 0;
    while (i < pScope->stmts.size())
    {
        if (pScope->stmts[i]->type == Scope::SCOPE)
        {
            cout << "Scope: " << pScope->stmts[i]->scope->name << endl;
            PrintInfo(pScope->stmts[i]->scope);
        }
        else
        {
            cout << "Statement: " << pScope->stmts[i]->LHS << " = " << pScope->stmts[i]->RHS << endl;
        }
        i++;
    }
}
