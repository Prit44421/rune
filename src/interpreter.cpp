#include "interpreter.h"

Value Interpreter::visit_literal(Literal & e){
    return e.val;
}

Value Interpreter::visit_group(GroupExpr & e){
    return evaluate(*e.exp);
}

Value Interpreter::visit_unary(UnaryExpr & e){
    Value r=evaluate(*e.num);
    if(e.o.type==TokenType::MINUS){
        if(holds_alternative<double>(r)){
            return -get<double>(r);
        }
        else{
            return  r;//todo error, not a number
        }
    }
    else{
        // todo not and error
    }
}

Value Interpreter::visit_binary(BinaryExpr& e){  //todo check for double
    Value l=evaluate(*e.left);
    Value r=evaluate(*e.right);
    switch(e.o.type){
        case(TokenType::PLUS):
            if(holds_alternative<double>(l) && holds_alternative<double>(r)){
                return get<double>(l)+get<double>(r);
            }
        case(TokenType::MINUS):
            return get<double>(l)-get<double>(r);
        case(TokenType::STAR):
            return get<double>(l)*get<double>(r);
        case(TokenType::SLASH):
            return get<double>(l)/get<double>(r);
        // case(TokenType::PERCENT):
        //     return get<double>(l)%get<double>(r);
        case(TokenType::GREATER):
            return get<double>(l)>get<double>(r);
        case(TokenType::GREATER_EQUAL):
            return get<double>(l)>=get<double>(r);
        case(TokenType::LESS):
            return get<double>(l)<get<double>(r);
        case(TokenType::LESS_EQUAL):
            return get<double>(l)<=get<double>(r);
        case(TokenType::EQUAL_EQUAL):
            return get<double>(l)==get<double>(r);
        case(TokenType::NOT_EQUAL):
            return get<double>(l)!=get<double>(r);
    }
}


Value Interpreter::visit_assign(AssignExpr & e){
    Value v=evaluate(*e.exp);
    env->assign(e.name,v);
    return v;
}

Value Interpreter::visit_logical(LogicalExpr &e){
    Value l=evaluate(*e.left);
    //todo complete it
}

Value Interpreter::visit_var(VarExpr &e){
    return env->get(e.name);
}

void Interpreter::visit_expr_stmt(ExprStmt &e){
    evaluate(*e.expression);
}

void Interpreter::visit_print(PrintStmt &e){
    Value exp=evaluate(*e.expression);
    string s=make_string(exp);
    cout<<s<<"\n";
}

void Interpreter::visit_var_decl(VarDeclStmt &e){
    Value ini=evaluate(*e.initializer);
    env->define(e.name.text,ini); //todo implement if token type also matters
}

void Interpreter::visit_block(BlockStmt & e){ //todo implement it
    shared_ptr<Environment> new_env=make_shared<Environment>(env);
    
    execute_block(e.statements,new_env);
    
}


void Interpreter::execute_block(vector<unique_ptr<Stmt>>& stmts, shared_ptr<Environment> new_env){
    shared_ptr<Environment> prev=env;
    env=new_env;
    for(auto& stmt : stmts){
        execute(*stmt);
    }
    env=prev;
}

void Interpreter::execute(Stmt &s){
    s.accept(*this);
}

Value Interpreter::evaluate(Expr &e){
    return e.accept(*this);
}

bool Interpreter::is_true(Value v){
    if(holds_alternative<bool>(v)){
        return get<bool>(v);
    }
    //todo error
}

string Interpreter::make_string(Value v){
    if(holds_alternative<string>(v)){
        return get<string>(v);
    }
    if(holds_alternative<bool>(v)){
        if(get<bool>(v)) return "true";
        return "false";
    }
    if(holds_alternative<double>(v)){
        return to_string(get<double>(v));
    }
    //todo error
}

void Interpreter::interpret(vector<unique_ptr<Stmt>>& stmts){
    for(auto& s:stmts){
        execute(*s);
    }
}

void Interpreter::visit_if(IfStmt &e){
    Value cond=evaluate(*e.condition);
    if(is_true(cond)){
        execute(*e.then_branch);
    }
    else{
        if(e.else_branch!=nullptr){
            execute(*e.else_branch);
        }
    }
}

void Interpreter::visit_while(WhileStmt &e){
    while(is_true(evaluate(*e.condition))){
        execute(*e.body);
    }
}

