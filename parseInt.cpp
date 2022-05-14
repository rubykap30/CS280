 * parse.cpp
 * Programming Assignment 2
 * Spring 2022
*/

#include "parseInt.h"

map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempResults;
queue <Value> *ValQue;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(istream& in, int& line);


//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line)
{
	bool f1, f2;
	LexItem tok = Parser::GetNextToken(in, line);
		
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			
			tok = Parser::GetNextToken(in, line);
			if (tok.GetToken() == SEMICOL) {
				f1 = DeclBlock(in, line); 
			
				if(f1) {
					f2 = ProgBody(in, line);
					if(!f2)
					{
						ParseError(line, "Incorrect Program Body.");
						return false;
					}
					
					return true;//Successful Parsing is completed
				}
				else
				{
					ParseError(line, "Incorrect Declaration Section.");
					return false;
				}
			}
			else
			{
				//Parser::PushBackToken(tok);
				ParseError(line-1, "Missing Semicolon.");
				return false;
			}
		}
		else
		{
			ParseError(line, "Missing Program Name.");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	ParseError(line, "Missing PROGRAM.");
	return false;
}

bool ProgBody(istream& in, int& line){
	bool status;
		
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == BEGIN) {
		
		status = Stmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
                cout << tok.GetLexeme() << endl;
				ParseError(line, "Missing semicolon in Statement.");
				return false;
			}
			
			status = Stmt(in, line);
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok == END )
		{
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Program Body.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Program Body.");
		return false;
	}	
}//End of ProgBody function


bool DeclBlock(istream& in, int& line) {
	bool status = false;
	LexItem tok;
	//cout << "in Decl" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	if(t == VAR)
	{
		status = DeclStmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Declaration Statement.");
				return false;
			}
			status = DeclStmt(in, line);
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok == BEGIN )
		{
			Parser::PushBackToken(tok);
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Declaration Block.");
		return false;
	}
	
}//end of DeclBlock function

bool DeclStmt(istream& in, int& line)
{
	LexItem t;
	bool status = IdentList(in, line);
	
	if (!status)
	{
		ParseError(line, "Incorrect variable in Declaration Statement.");
		return status;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t == COLON)
	{
		t = Parser::GetNextToken(in, line);
        // TO ADD INTO SYMTABLE DECLARED VARIABLES
		if(t == INTEGER || t == REAL || t == STRING)
		{
            for(auto i = defVar.begin(); i!=defVar.end(); i++){
                if(i -> second && (SymTable.find(i->first) == SymTable.end())){
                    SymTable[i->first] = t.GetToken();
                }
            }
            return true;
           
        }
		else
		{
			ParseError(line, "Incorrect Declaration Type.");
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(t);
		return false;
	}
	
}//End of DeclStmt

//IdList:= IDENT {,IDENT}
bool IdentList(istream& in, int& line) {
	bool status = false;
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = IdentList(in, line);
	}
	else if(tok == COLON)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return status;
	
}//End of IdentList
	

//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
//Stmt = AssigStmt | IfStmt | WriteStmt | ForStmt 
bool Stmt(istream& in, int& line) {
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case WRITELN:
		status = WriteLnStmt(in, line);
		
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		
		break;
		
		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}//End of Stmt


//WriteStmt:= wi, ExpreList 
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	ValQue = new queue <Value>;	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression after WriteLn");
        while(!(*ValQue).empty()){
            ValQue -> pop();
        }
        delete ValQue;
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	while(!(*ValQue).empty()){
        Value nextVal = (*ValQue).front();
        cout << nextVal;
        ValQue -> pop();
    }
    cout << endl;
	return ex;
}

//IfStmt:= if (Expr) then Stm} [Else Stmt]
bool IfStmt(istream& in, int& line) {
    Value v1;
	bool ex=false, status ; 
	LexItem t;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	ex = LogicExpr(in, line, v1);
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != THEN)
	{
		ParseError(line, "If-Stmt Syntax Error");
		return false;
	}
    if(v1.GetBool()){
        status = Stmt(in, line);
        if(!status) {
            ParseError(line, "Missing Statement for If-Stmt Then Part");
            return false;
        }
        while(t!= SEMICOL){
            t= Parser::GetNextToken(in,line);
        }
    }
    while(t != ELSE && t != SEMICOL){
        t = Parser::GetNextToken(in,line);
    }

	//t = Parser::GetNextToken(in, line);
	if( t == ELSE || v1.GetBool() == false) {
		status = Stmt(in, line);
		if(!status)
		{
			ParseError(line, "Missing Statement for If-Stmt Else-Part");
			return false;
		}
		//cout << "in IFStmt status of Stmt true" << endl;
		return true;
	}
		
	Parser::PushBackToken(t);
	return true;
}//End of IfStmt function



//Var:= ident
bool Var(istream& in, int& line, LexItem& idtok)
{
	//called only from the AssignStmt function
	string identstr;
	
	idtok = Parser::GetNextToken(in, line);
	
	if (idtok == IDENT){
		identstr = idtok.GetLexeme();
		
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}	
		return true;
	}
	else if(idtok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << idtok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}//End of Var

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	Value v1;
	bool varstatus = false, status = false;
	LexItem t;
	LexItem u;
	varstatus = Var(in, line, u);
	
	
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		if (t == ASSOP){
			status = Expr(in, line, v1);
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
        
		if(v1.GetType() == VERR){
            ParseError(line, "Illegal Assignment Operation");
            return false;
        }
        
        TempResults[u.GetLexeme()] = v1;
        if(SymTable[u.GetLexeme()] == STRING){
            if(!v1.IsString()){
            ParseError(line,"Illegal Assignment operations");
            return false;
            }
		}
        else{
            if(v1.IsString() || v1.IsBool()){
             ParseError(line,"Illegal Assignment operations -- first case");
             return false;
            }
            if(SymTable[u.GetLexeme()] == INTEGER && v1.IsReal()){
                //typecast and then set value to new item
                v1= (int)v1.GetReal();
                TempResults[u.GetLexeme()].SetType(VINT);
            }
            if(SymTable[u.GetLexeme()] == REAL && v1.IsInt()){
                //MAKE TEMP VALS HERE FOR VALUE watch out for scoping
                v1=(float)v1.GetInt();
                TempResults[u.GetLexeme()].SetType(VREAL);
            }
                
        }
        }
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
    TempResults[u.GetLexeme()] = v1;
	return status;	

}

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value v1;
	status = Expr(in, line, v1);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue -> push(v1);
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value& retVal) {
	Value v1,v2;
	bool t1 = Term(in, line, v1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
    retVal = v1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS ) 
	{
		t1 = Term(in, line, v2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == PLUS){
            retVal = retVal + v2;
            if(retVal.IsErr()){
                ParseError(line,"Mixed type operands for addition");
                return false;
            }
        }
        else if(tok == MINUS){
            retVal = retVal - v2;
            if(retVal.IsErr()){
                ParseError(line, "Mixed type operands for subtraction");
                return false;
            }
        }
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}

//Term:= SFactor {(*|/) SFactor}
bool Term(istream& in, int& line, Value& retVal) {
	Value v1, v2;
	bool t1 = SFactor(in, line, v1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = v1;
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  )
	{
		t1 = SFactor(in, line, v2);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == MULT){
            retVal = retVal * v2;
            if(retVal.IsErr()){
                ParseError(line, "Mixed type operands for multiplication");
                return false;
            }
        }
        else if(tok == DIV){
             if(v2.IsReal()){
                 if(v2.GetReal() == 0){
                     ParseError(line,"Divison by 0");
                     return false;
                 }
             }
             else if(v2.IsInt()){
                 if(v2.GetInt() == 0){
                     ParseError(line,"Divison by 0- Int");
                     return false;
                 }
             }
             
                 retVal = retVal/v2;
                 if(retVal.IsErr()){
                 ParseError(line,"Error in divison");
                 return false;
             }
            
         }    
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

//SFactor = Sign Factor | Factor
bool SFactor(istream& in, int& line, Value& retVal)
{
    Value v1;
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else{
		Parser::PushBackToken(t);
    }
	status = Factor(in, line, sign, v1);
    if(status){
        retVal = v1;
    }
	return status;
}

//LogicExpr = Expr (== | <) Expr
bool LogicExpr(istream& in, int& line, Value& retVal)
{
    Value v1, v2;
	bool t1 = Expr(in, line, v1);
	LexItem tok;
    retVal.SetType(VBOOL);
    
	
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == GTHAN  || tok == EQUAL  || tok == LTHAN)
	{
		t1 = Expr(in, line, v2);
		if( !t1 ) 
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
        if(tok == EQUAL)
        {
            retVal = v1 == v2;
            if(retVal.IsErr()){
                 ParseError(line,"Error in equality");
                 return false;
             }
        }
        else if (tok == LTHAN){
            retVal  = v1 < v2;
            if(retVal.IsErr()){
                 ParseError(line,"Error in less than");
                 return false;
             }
        }
        else if(tok == GTHAN){
            retVal = v1 > v2;
            if(retVal.IsErr()){
                 ParseError(line,"Error in greater than");
                 return false;
             }
        }
		return true;
	}
	Parser::PushBackToken(tok);
	return true;
}

//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, int sign, Value & retVal)
{
    LexItem tok = Parser::GetNextToken(in, line);
    if( tok == IDENT ) {
        string lexeme = tok.GetLexeme();
        if (!(defVar.find(lexeme)->second))
        {
            ParseError(line, "Undefined Variable");
            return false;
        }
        if (TempResults.find(lexeme) == TempResults.end()) {
            ParseError(line, "Undefined Variable");
            return false;
        }
        retVal = TempResults[lexeme];
        if (sign == -1){
            if(retVal.IsReal()){
                retVal.SetReal(-(retVal.GetReal()));
            }
            if(retVal.IsInt()){
                retVal.SetInt(-(retVal.GetInt()));
            }
        }
        return true;
    }
    else if( tok == ICONST ) {
        if (sign == -1){
            retVal = Value(-(stoi(tok.GetLexeme())));
        }
        else{
            retVal = Value(stoi(tok.GetLexeme()));
        }
        return true;
    }
    else if( tok == SCONST) {
        if (tok == SCONST && sign !=0){
            ParseError(line, "String has a sign in it");
            return false;
        }
        retVal = Value(tok.GetLexeme());
        return true;
    }
    else if( tok == RCONST ) {
        if(sign == -1){
            retVal = Value(-(stof(tok.GetLexeme())));
        }
        else{
            retVal = Value(stof(tok.GetLexeme()));
        }
        return true;
    }
    else if( tok == LPAREN ) {
        bool ex = Expr(in, line, retVal);
        if( !ex ) {
            ParseError(line, "Missing expression after (");
            return false;
        }
        if( Parser::GetNextToken(in, line) == RPAREN )
        {
            return ex;
        }
        ParseError(line, "Missing ) after expression");
        return false;
    }
    else if(tok.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    ParseError(line, "Unrecognized input");
    return true;
}




