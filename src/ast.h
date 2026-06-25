#pragma once

#include <variant>
#include <string>
#include <memory>
#include <vector>
#include "tokens.h"
using namespace std;

struct Nil{
};

using Value = variant<Nil, double, string, bool>;

struct Literal;
struct BinaryExpr;
struct UnaryExpr;
struct GroupExpr;
struct VarExpr;
struct AssignExpr;
struct LogicalExpr; // todo
struct CallExpr;    // todo

struct ExprVisitor{
    ~ExprVisitor() = default;
    virtual Value visit_literal(Literal &expr) = 0;
    virtual Value visit_binary(BinaryExpr &expr) = 0;
    virtual Value visit_unary(UnaryExpr &expr) = 0;
    virtual Value visit_group(GroupExpr &expr) = 0;
    virtual Value visit_var(VarExpr &expr) = 0;
    virtual Value visit_assign(AssignExpr &expr) = 0;
    virtual Value visit_logical(LogicalExpr &expr) = 0;
};

struct Expr{
    ~Expr() = default;

    virtual Value accept(ExprVisitor &visitor) = 0;
};

struct Literal : Expr{
    Value val;
    Literal(Value v)
    {
        val = v;
    }
    Value accept(ExprVisitor &visitor) override{
        return visitor.visit_literal(*this);
    }
};

struct BinaryExpr : Expr{
    unique_ptr<Expr> left;
    unique_ptr<Expr> right;
    Token o;

    BinaryExpr(unique_ptr<Expr> l, Token op, unique_ptr<Expr> r):
        left(move(l)),
        right(move(r)),
        o(op)
    {}
    Value accept(ExprVisitor &visitor) override{
        return visitor.visit_binary(*this);
    }
};

struct UnaryExpr : Expr{
    Token o;
    unique_ptr<Expr> num;

    UnaryExpr(Token op, unique_ptr<Expr> n):
        o(op),
        num(move(n))
    {}
    Value accept(ExprVisitor &visitor) override{
        return visitor.visit_unary(*this);
    }
};

struct GroupExpr : Expr{

    unique_ptr<Expr> exp;
    GroupExpr(unique_ptr<Expr> e)
    {
        exp = move(e);
    }

    Value accept(ExprVisitor &visitor) override{
        return visitor.visit_group(*this);
    }
};

struct VarExpr : Expr{

    Token name;

    VarExpr(Token n):
        name(n)
    {}

    Value accept(ExprVisitor &visitor) override{
        return visitor.visit_var(*this);
    }
};

struct AssignExpr : Expr{

    Token name;
    unique_ptr<Expr> exp;

    AssignExpr(unique_ptr<Expr> e, Token t):
        exp(move(e)),
        name(t)
    {}

    Value accept(ExprVisitor &visitor) override{

        return visitor.visit_assign(*this);
    }
};

struct LogicalExpr : Expr{
    unique_ptr<Expr> left;
    TokenType op;
    unique_ptr<Expr> right;

    LogicalExpr(unique_ptr<Expr> l, TokenType o, unique_ptr<Expr> r):
        left(move(l)),
        op(o),
        right(move(r))
    {}

    Value accept(ExprVisitor &visitor) override{
        return visitor.visit_logical(*this);
    }
};

struct ExprStmt;
struct PrintStmt;
struct VarDeclStmt;
struct BlockStmt;
struct IfStmt;
struct WhileStmt;
struct FunDeclStmt;
struct ReturnStmt;

struct StmtVisitor{
    virtual ~StmtVisitor() = default;
    virtual void visit_expr_stmt(ExprStmt &stmt) = 0;
    virtual void visit_print(PrintStmt &stmt) = 0;
    virtual void visit_var_decl(VarDeclStmt &stmt) = 0;
    virtual void visit_block(BlockStmt &stmt) = 0;
    virtual void visit_if(IfStmt &stmt) = 0;
    virtual void visit_while(WhileStmt &stmt) = 0;
    // virtual void visit_fun_decl(FunDeclStmt &stmt) = 0;
    // virtual void visit_return(ReturnStmt &stmt) = 0;
};

struct Stmt{
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor &visitor) = 0;
};

struct ExprStmt : Stmt{
    unique_ptr<Expr> expression;
    ExprStmt(unique_ptr<Expr> e){
        expression = move(e);
    }
    void accept(StmtVisitor &visitor) override{
        visitor.visit_expr_stmt(*this);
    }
};

struct PrintStmt : Stmt{
    unique_ptr<Expr> expression;
    PrintStmt(unique_ptr<Expr> e){
        expression = move(e);
    }
    void accept(StmtVisitor &visitor) override{
        visitor.visit_print(*this);
    }
};

struct VarDeclStmt : Stmt{
    Token name;
    unique_ptr<Expr> initializer;
    VarDeclStmt(Token n, unique_ptr<Expr> init):
        name(n),
        initializer(move(init))
    {}
    void accept(StmtVisitor &visitor) override{
        visitor.visit_var_decl(*this);
    }
};

struct BlockStmt : Stmt{
    vector<unique_ptr<Stmt>> statements;
    BlockStmt(vector<unique_ptr<Stmt>> s){
        statements = move(s);
    }
    void accept(StmtVisitor &visitor) override{
        visitor.visit_block(*this);
    }
};

struct IfStmt : Stmt{
    unique_ptr<Expr> condition;
    unique_ptr<Stmt> then_branch;
    unique_ptr<Stmt> else_branch;

    IfStmt(unique_ptr<Expr> cond, unique_ptr<Stmt> then_b, unique_ptr<Stmt> else_b){
        condition = move(cond);
        then_branch = move(then_b);
        else_branch = move(else_b);
    }

    void accept(StmtVisitor &visitor) override{
        visitor.visit_if(*this);
    }
};

struct WhileStmt : Stmt{
    unique_ptr<Expr> condition;
    unique_ptr<Stmt> body;

    WhileStmt(unique_ptr<Expr> cond, unique_ptr<Stmt> b){
        condition = move(cond);
        body = move(b);
    }

    void accept(StmtVisitor &visitor) override{
        visitor.visit_while(*this);
    }
};

struct FunDeclStmt : Stmt{
    Token name;
    vector<Token> params;
    vector<unique_ptr<Stmt>> body;

    FunDeclStmt(Token n, vector<Token> p, vector<unique_ptr<Stmt>> b):
        name(n),
        params(p),
        body(move(b))
    {}

    void accept(StmtVisitor &visitor) override{
        // visitor.visit_fun_decl(*this);
    }
};

struct ReturnStmt : Stmt{
    Token keyword;
    unique_ptr<Expr> value;

    ReturnStmt(Token k, unique_ptr<Expr> v):
        keyword(k),
        value(move(v))
    {}

    void accept(StmtVisitor &visitor) override{
        // visitor.visit_return(*this);
    }
};
