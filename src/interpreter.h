
#include "ast.h"
#include "environment.h"

using namespace std;

class Interpreter: public ExprVisitor, public StmtVisitor{
private:

    shared_ptr<Environment> globals;
    shared_ptr<Environment> env;

    Value visit_literal(Literal &expr);
    Value visit_binary(BinaryExpr &expr);
    Value visit_unary(UnaryExpr &expr);
    Value visit_group(GroupExpr &expr);
    Value visit_var(VarExpr &expr);
    Value visit_assign(AssignExpr &expr);
    Value visit_logical(LogicalExpr &expr);
    Value visit_call(CallExpr &expr);


    void visit_expr_stmt(ExprStmt &stmt);
    void visit_print(PrintStmt &stmt);
    void visit_var_decl(VarDeclStmt &stmt);
    void visit_block(BlockStmt &stmt);
    void visit_if(IfStmt &stmt);
    void visit_while(WhileStmt &stmt);
    void visit_fun_decl(FunDeclStmt &stmt);
    void visit_return(ReturnStmt &stmt);
    
    
public:
    void interpret(vector<unique_ptr<Stmt>>& stmts);
    void execute(Stmt &s);
    Value evaluate(Expr &exp);
    void execute_block(vector<unique_ptr<Stmt>>& stmts, shared_ptr<Environment> e);
    bool is_true(Value v);
    string make_string(Value v);
    Interpreter(){
        env=make_shared<Environment>();
    }
};

struct ReturnExp{
    Value value;
};