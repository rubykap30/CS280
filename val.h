#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <iomanip>

using namespace std;

enum ValType { VINT, VREAL, VSTRING, VBOOL, VERR };

class Value {
    ValType	T;
    bool    Btemp;
	int		Itemp;
	float   Rtemp;
    string	Stemp;
       
public:
    Value() : T(VERR), Btemp(false), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(bool vb) : T(VBOOL), Btemp(vb), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(int vi) : T(VINT), Btemp(false), Itemp(vi), Rtemp(0.0), Stemp("") {}
	Value(float vr) : T(VREAL), Btemp(false), Itemp(0), Rtemp(vr), Stemp("") {}
    Value(string vs) : T(VSTRING), Btemp(false), Itemp(0), Rtemp(0.0), Stemp(vs) {}
    
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsString() const { return T == VSTRING; }
    bool IsReal() const {return T == VREAL;}
    bool IsBool() const {return T == VBOOL;}
    
    int GetInt() const { if( IsInt() ) return Itemp; throw "RUNTIME ERROR: Value not an integer"; }
    
    string GetString() const { if( IsString() ) return Stemp; throw "RUNTIME ERROR: Value not a string"; }
    
    float GetReal() const { if( IsReal() ) return Rtemp; throw "RUNTIME ERROR: Value not an integer"; }
    
    bool GetBool() const {if(IsBool()) return Btemp; throw "RUNTIME ERROR: Value not a boolean";}
    
    void SetType(ValType type)
    {
    	T = type;
	}
	
    void SetInt(int val)
    {
    	Itemp = val;
	}
	
	void SetReal(float val)
    {
    	Rtemp = val;
	}
	
	void SetString(string val)
    {
    	Stemp = val;
	}
	
	void SetBool(bool val)
    {
    	Btemp = val;
	}
	
	
    // add op to this
    Value operator+(const Value& op) const{
        bool valReal = this ->IsReal();
        bool nextReal = op.IsReal();
        bool valInt = this->IsInt();
        bool nextInt = op.IsInt();
        bool valString = this -> IsString();
        bool nextString = op.IsString();
        
        if(valReal && nextReal){
            return this->GetReal() + op.GetReal();
        }
        if(valInt && nextInt){
            return this->GetInt() + op.GetInt();
        }
        if(valReal && nextInt){
            return this->GetReal() + op.GetInt();
        }
        if(valInt && nextReal){
            return this->GetInt() + op.GetReal();
        }
        if(valString && nextString){
            return this-> GetString() + op.GetString();
        }
            return Value();
    }
    
    // subtract op from this
    Value operator-(const Value& op) const{
        bool valReal = this ->IsReal();
        bool nextReal = op.IsReal();
        bool valInt = this->IsInt();
        bool nextInt = op.IsInt();
        
        if(valReal && nextReal){
            return this->GetReal() - op.GetReal();
        }
        if(valInt && nextInt){
            return this->GetInt() - op.GetInt();
        }
        if(valReal && nextInt){
            return this->GetReal() - op.GetInt();
        }
        if(valInt && nextReal){
            return this->GetInt() - op.GetReal();
        }
            return Value();
    }
    
    // multiply this by op
    Value operator*(const Value& op) const{
        bool valReal = this ->IsReal();
        bool nextReal = op.IsReal();
        bool valInt = this->IsInt();
        bool nextInt = op.IsInt();
        
        if(valReal && nextReal){
            return this->GetReal() * op.GetReal();
        }
        if(valInt && nextInt){
            return this->GetInt() * op.GetInt();
        }
        if(valReal && nextInt){
            return this->GetReal() * op.GetInt();
        }
        if(valInt && nextReal){
            return this->GetInt() * op.GetReal();
        }
            return Value();
    }
    
    // divide this by op
    Value operator/(const Value& op) const {
        bool valReal = this ->IsReal();
        bool nextReal = op.IsReal();
        bool valInt = this->IsInt();
        bool nextInt = op.IsInt();
        if(valReal && nextReal){
            return this->GetReal() / op.GetReal();
        }
        if(valInt && nextInt){
            return this->GetInt() / op.GetInt();
        }
        if(valReal && nextInt){
            return this->GetReal() / op.GetInt();
        }
        if(valInt && nextReal){
            return this->GetInt() / op.GetReal();
        }
            return Value();
    }
    
    Value operator==(const Value& op) const{
        bool valReal = this ->IsReal();
        bool nextReal = op.IsReal();
        bool valInt = this->IsInt();
        bool nextInt = op.IsInt();
        bool valString = this -> IsString();
        bool nextString = op.IsString();
        
        if(valReal && nextReal){
            return this->GetReal() == op.GetReal();
        }
        if(valInt && nextInt){
            return this->GetInt() == op.GetInt();
        }
        if(valReal && nextInt){
            return this->GetReal() == op.GetInt();
        }
        if(valInt && nextReal){
            return this->GetInt() == op.GetReal();
        }
        if(valString && nextString){
            return this-> GetString() == op.GetString();
        }
            return Value();
    }

	Value operator>(const Value& op) const{
        bool valReal = this ->IsReal();
        bool nextReal = op.IsReal();
        bool valInt = this->IsInt();
        bool nextInt = op.IsInt();
        bool valString = this -> IsString();
        bool nextString = op.IsString();
        
        if(valReal && nextReal){
            return this->GetReal() > op.GetReal();
        }
        if(valInt && nextInt){
            return this->GetInt() > op.GetInt();
        }
        if(valReal && nextInt){
            return this->GetReal() > op.GetInt();
        }
        if(valInt && nextReal){
            return this->GetInt() > op.GetReal();
        }
        if(valString && nextString){
            return this-> GetString() > op.GetString();
        }
            return Value();
    }

	
	Value operator<(const Value& op) const {
        bool valReal = this ->IsReal();
        bool nextReal = op.IsReal();
        bool valInt = this->IsInt();
        bool nextInt = op.IsInt();
        bool valString = this -> IsString();
        bool nextString = op.IsString();
        
        if(valReal && nextReal){
            return this->GetReal() < op.GetReal();
        }
        if(valInt && nextInt){
            return this->GetInt() < op.GetInt();
        }
        if(valReal && nextInt){
            return this->GetReal() < op.GetInt();
        }
        if(valInt && nextReal){
            return this->GetInt() < op.GetReal();
        }
        if(valString && nextString){
            return this-> GetString() < op.GetString();
        }
            return Value();
    }
	
	    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.Itemp;
        else if( op.IsString() ) out << op.Stemp;
        else if( op.IsReal()) out << fixed << showpoint << setprecision(2) << op.Rtemp;
        else if (op.IsBool()) {
        	if(op.GetBool()) cout << "true" ;
        	else
        		cout << "false" ;
		}
        else out << "ERROR";
        return out;
    }
};


#endif

