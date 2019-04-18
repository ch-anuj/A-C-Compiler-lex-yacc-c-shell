%{
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <unistd.h>
using namespace std;
extern int yylex();
extern int yylineno;

void yyerror (const char* s) {
  cout<<s<<"in line:"<<yylineno<<endl;
  exit(0);
}

// -----------------------------------
typedef struct A{
  char label[25];
  int id;
  struct A *next;
  struct B *right;
}A;
typedef struct B
{
  int id;
  struct B *next;
}B;
typedef struct C
{
	char varName[25];
	struct C *next;
}C;
typedef struct parameters
{
	int type;
	struct parameters *next;
}parameters;
typedef struct lst
{
	char var_name[25];
	char type[7];
	struct lst * next;
}lst;
typedef struct gst{
	char f_name[25];
	struct lst* f_tableHead;
	struct lst* f_tableTail;
	char ret_type[7];
	struct gst* next;
	int nParam;
	parameters* p_list;
}gst;
int searchResult,nodeId=1,temp1,temp2,temp3,temp4,temp5,temp6,temp7;
A* headA = NULL;
A* tailA = NULL;
C* headC = NULL;
C* tailC = NULL;
C* headC1 = NULL;
C* tailC1 = NULL;
gst *PtrGst=NULL;
lst *PtrLst=NULL;
gst* headGst=NULL;
gst* tailGst=NULL;
char functionType[7];
char currentFun[25];
char currType[10];
ofstream myfile;
ofstream tableFile;
void emit(char* s){
	cout<<"|";
	cout<<s;
	cout<<"|";


  myfile <<s<<" ";

}
int lookupGst(char* fooName,gst* headGst){
	//cout<<"entering lookupGst";
  gst* temp = headGst;
  while(temp!=NULL){
    if(strcmp(temp->f_name,fooName)==0){
    	PtrGst = temp;
    //	cout<<"exiting lookupGst";
    	return 1;
    }
    temp = temp->next;
  }
 // cout<<"exiting lookupGst";
  return 0;
}

void insert_gst(char* fooName,gst** tailGst,gst** headGst){
//	cout<<"entering 2 ";
	int k;
	k=lookupGst(fooName,*headGst);
	if(k==1){
		cout<<"error! (duplicate insertion tried)";
		return;
	}
  gst* temp = (gst*)malloc(sizeof(gst));
  temp->next = NULL;
  temp->f_tableHead=NULL;
  temp->f_tableTail=NULL;
  temp->p_list = NULL;
  strcpy(temp->f_name,fooName);
  if((*tailGst)!=NULL){
  	// gst table is not empty
    (*tailGst)->next = temp;
  }
  (*tailGst) = temp;
  if((*headGst)==NULL)(*headGst) = (*tailGst);
 // cout<<"exiting 2 ";
  return;
}
int lookupLst(char* varName,lst* headLst){
//	cout<<"entering 3 ";
  lst* temp = headLst;
  while(temp!=NULL){
    if(strcmp(temp->var_name,varName)==0){
    	PtrLst = temp;
    //	cout<<"exiting 3 ";
    	return 1;
    }
    temp = temp->next;
  }
  //cout<<"exiting 3 ";
  return 0;
}
void insert_lst(char* varName,lst** tailLst,lst** headLst){
//	cout<<"entering 4 ";
	int k;
	k=lookupLst(varName,*headLst);
	if(k==1){
		cout<<"error! (duplicate insertion tried)";
    exit(0);
		return;
	}
  lst* temp = (lst*)malloc(sizeof(lst));
  temp->next = NULL;
  strcpy(temp->var_name,varName);
  if((*tailLst)!=NULL){
  	// lst table is not empty
    (*tailLst)->next = temp;
  }
  (*tailLst) = temp;
  if((*headLst)==NULL)(*headLst) = (*tailLst);
 // cout<<"exiting 4 ";
  return;
}
void printlistP(parameters* headP){
  if(headP==NULL)return;
  parameters* temp = headP;
  while(temp!=NULL){
    cout<<temp->type<<" ";
    temp = temp->next;
  }
}

void printGst(gst* headA){
	if(headA==NULL) return;
  cout<<"\n--printing gst----\n";
  gst* temp = headA;
  while(temp!=NULL){
    if(temp->p_list==NULL)
    cout<<temp->f_name<<" "<<temp->ret_type<<" "<<temp->nParam<<endl;
    else{
        cout<<temp->f_name<<" "<<temp->ret_type<<" "<<temp->nParam;
        cout<<"(";
        printlistP(temp->p_list);
        cout<<")";
        cout<<endl;
    }

    temp=temp->next;
  }
  cout<<"\n------\n";
}
void printLst(lst* headA){
	if(headA==NULL)return;
  cout<<"\n--printing lst----\n";
  lst* temp = headA;
  while(temp!=NULL){
    cout<<temp->type<<" "<<temp->var_name<<" "<<endl;
    temp=temp->next;
  }
  cout<<"\n------\n";
}
int setPtrGst(char* fooName){
//	cout<<"entering 5 ";
	lookupGst(fooName,headGst);
	//cout<<"exiting 5 ";
	if(PtrGst!=NULL)return 1;
	else return 0;
}
int setPtrLst(char* varName){
//	cout<<"entering 6 ";
	if(PtrGst==NULL){
		cout<<"error in setting ptrlst because ptrgst is null!";
		return 0;
	}
	lookupLst(varName,PtrGst->f_tableHead);
//	cout<<"exiting 6 ";
	if(PtrLst!=NULL)return 1;
	else return 0;
}
int setBothPtr(char* fooName,char* varName){
  int k;
  k=setPtrGst(fooName);
  if(k==0)return 0;
  else{
    k=setPtrLst(varName);
    if(k==0)return 0;
    else
      return 1;
  }
}
void printAllTables(gst* headGst){
//	cout<<"entering 7 ";
	gst* temp = headGst;
	printGst(headGst);
  	while(temp!=NULL){
    	printLst(temp->f_tableHead);
    temp=temp->next;
  }
//  cout<<"exiting 7 ";

}
void printGraph(A* headA){
  cout<<"strict graph G {\n";
  A* temp = headA;
  while(temp!=NULL){
    cout<<temp->id<<" [label=\""<<temp->label<<"\"];"<<endl;
    temp=temp->next;
  }
  temp = headA;
  B* helper;
  while(temp!=NULL){
    if(temp->right!=NULL){
      helper = temp->right;
      cout<<temp->id<<"--";
      while(helper!=NULL){
        if(helper->next!=NULL)
          cout<<helper->id<<", ";
        else
          cout<<helper->id<<" ";
        helper = helper->next;
      }
      cout<<";"<<endl;
    }
    temp = temp->next;
  }
  cout<<"\n}\n";
}
void addNodeC(char* label,C** tailC,C** headC){
//	cout<<"adding node "<<label<<" to C \n";

  C* temp = (C*)malloc(sizeof(C));
  temp->next = NULL;
  strcpy(temp->varName,label);
  if((*tailC)!=NULL){
    (*tailC)->next = temp;
    (*tailC) = (*tailC)->next;
  }
  else{
  	(*tailC) = temp;
  }


  if((*headC)==NULL)(*headC) = (*tailC);
//  cout<<"exiting 8 ";
  return ;
}
void flush(C** headC){
//	cout<<"entering 9 ";
	C* temp = *headC;
	C* temp1;
	if(*headC==NULL)return;
	char np[25];
	strcpy(np,(*headC)->varName);
	temp=temp->next;
	while(temp!=NULL){
		insert_lst(temp->varName,&(PtrGst->f_tableTail),&(PtrGst->f_tableHead));
		temp=temp->next;
	}
	 temp=(*headC)->next;
	  while(temp!=NULL){
	  	setPtrLst(temp->varName);
	  	strcpy(PtrLst->type,np);
      temp=temp->next;
	  }
	 while((*headC)!=NULL){
	 	temp1 = (*headC);
	 	(*headC)=(*headC)->next;
	 	free(temp1);
	 }
   *headC=NULL;
//	cout<<"exiting 9 ";
}
int findNode(int id,A* headA){
  A* temp = headA;
  while(temp!=NULL){
    if(temp->id==id)return 1;
    temp = temp->next;
  }
  return 0;
}
// makeChild to be called only if both parent and child are already created in the graph
void makeChild(int parent,int child,A* headA){
  A* p=headA;
  B* temp;
  if(p==NULL)return;
  int flagC = 0;
  while(p!=NULL){
    if(p->id==parent)break;
    p = p->next;
  }
  if(p==NULL)return;
  B* c = p->right;
  while(1){
    if(c!=NULL){
      if(c->next==NULL) break;
      else{
        c = c->next;
      }
    }
    else{
      flagC = 1;
      break;
    }
  }
  temp = (B*)malloc(sizeof(B));
  temp->next = NULL;
  temp->id = child;
  if(flagC==0){
    //there is at least one neighbour of the parent
    c->next = temp;
  }
  else{
    p->right = temp;
  }

}
void printlistC(C* headC){
	cout<<"entering printlistC\n";
	if(headC==NULL)return;
	C* temp = headC;
  while(temp!=NULL){
    cout<<temp->varName<<endl;
    temp = temp->next;
  }
  cout<<"\nexiting printlistC";
}

int length(C* head)
{
    int count = 0;
    C* current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}
parameters* allocate(int k) {
    if (k==0) return NULL;
    parameters* result = (parameters*)malloc(sizeof(parameters));
    result->type=7;
    result->next = allocate(k-1);
    return result;
}
void flush2(C** headC){
 // cout<<"\nentering flush2 ";
  C* temp = *headC;
  C* temp1;
  parameters* tempP;
  if(*headC==NULL)return;
  PtrGst->nParam=length(*headC);
  PtrGst->p_list=allocate(length(*headC));
  tempP=PtrGst->p_list;
  while(temp!=NULL){
    if(strcmp(temp->varName,strdup("int"))==0){
        tempP->type = 1;
    }
    if(strcmp(temp->varName,strdup("char"))==0){
        tempP->type = 2;
    }
    if(strcmp(temp->varName,strdup("void"))==0){
        tempP->type = 3;
    }
    temp=temp->next;
    tempP=tempP->next;
  }

   while((*headC)!=NULL){
    temp1 = (*headC);
    (*headC)=(*headC)->next;
    free(temp1);
   }
   *headC=NULL;
 // cout<<"\nexiting flush2 ";
}
// ----------------------------------
////////////////anuj functions///////////////////////

int tempCounter = 0;
int labelCounter = 0;
int funcCounter = 0;
char buffer[1000];

char* newTempGen(){
  char buffer[50];
  char* a = (char*)malloc(10*sizeof(char));
  sprintf(buffer, "t.%d", tempCounter);
  tempCounter= tempCounter + 1;
  strcpy(a,buffer);
  return a;
}

char* newLabelGen(){
  char buffer[50];
  char* a = (char*)malloc(10*sizeof(char));
  sprintf(buffer, "L~%d", labelCounter);
  labelCounter++;
  strcpy(a,buffer);
  return a;
}
int lookAllTable(char* name,gst* headGst){
  int a,b;
  gst* temp;
  a=lookupGst(name,headGst);
  if(a==1)return 1;
  else{
    temp = headGst;
    while(temp!=NULL){
      b=lookupLst(name,temp->f_tableHead);
      if(b==1)return 1;
      temp = temp->next;
    }
    return 0;
  }
}
void TableDump(gst* headGst){
  freopen("table.csv","w",stdout);
  gst* temp = headGst;
  printGst(headGst);
    while(temp!=NULL){
      printLst(temp->f_tableHead);
    temp=temp->next;
  }
//  cout<<"exiting 7 ";

}

%}


%union {
  struct{
  char val[25];
  int nodeId;
  char ircode[10000];
  }value;
}


%token <value> IF O_BRAC C_BRAC OPEN_BRAC CLOSE_BRAC MAIN THEN PERCENT ENDIF NUM CHARACTER
%token <value> SEMICOLON COLON DOT AMPERSAN PRECENT BACKSLASH ELSE SCAN PRINT INT CHAR FLOAT WHILE GOTO SWITCH VOID
%token <value> BREAK CASE CONTINUE RETURN COMMA LETTER NUMBER INTEGER FLOATING_INT IDENTIFIER
%token <value> Obrac Cbrac
%token <value> EQUALS NOT DO FOR EXIT
%token <value> OR EQEQ NEQ G_THAN L_THAN G_EQ L_EQ AND PLUS MINUS MULTIPLY DIVIDE


%type <value> main_unit main_decl func_decls func_decl body type_spec param_list param_decl mainkeyword fun_type_spec fun_name
%type <value> decl_spec declarator declarators init_decl_list declare decl_list MainParamList
%type <value> compound_stmt expr_stmt stmt_list stmt iterate_stmt log_or_expr jump_stmt empty
%type <value> func_call_stmt without_return with_return call_param_list non_empty_param_list empty_param_list
%type <value> if_stmt primary_expr mul_expr add_expr relational_expr print_stmt scan_stmt equality_expr log_and_expr assgn_expr
%type <value> Pdecl_spec
%right  EQUALS NOT
%left  OR EQEQ NEQ G_THAN L_THAN G_EQ L_EQ AND PLUS MINUS MULTIPLY DIVIDE



%%
main_unit
      : func_decls main_decl{
                  cout<<"1 ";
                  cout<<"succesfully parsed(1)";
                  printAllTables(headGst);
                  strcpy($$.ircode,$1.ircode);
                  sprintf($$.ircode, "%s\n%s", $1.ircode, $2.ircode);
                  emit($$.ircode);

      }
      | main_decl{
                  cout<<"71 ";
                  cout<<"succesfully parsed(2)";
                  printAllTables(headGst);
                  strcpy($$.ircode,$1.ircode);
                  emit($$.ircode);

      }
      ;
func_decls
      : func_decl  {
                  cout<<"43 ";
                  strcpy($$.ircode,$1.ircode);

      }
      | func_decls func_decl  {
                  cout<<"45 ";
                  sprintf($$.ircode,"%s\n%s", $1.ircode, $2.ircode);

      }
      ;
main_decl
      : fun_type_spec mainkeyword O_BRAC empty C_BRAC body
      {
                  cout<<"3 ";
                  funcCounter++;
                  sprintf($$.ircode, "@%s:\n%s", $2.ircode,$6.ircode);
                  // emit($$.ircode);

      }
      ;

// main_body
//       : OPEN_BRAC alterd_stmt RETURN SEMICOLON CLOSE_BRAC{
//                   cout<<"65 ";
//                   sprintf($$.ircode, "%s\nreturn", $2.ircode);
//                   }
//       | OPEN_BRAC alterd_stmt RETURN log_or_expr SEMICOLON CLOSE_BRAC{
//                   cout<<"67 ";
//                   sprintf($$.ircode, "%s\n%s\nreturn %s", $2.ircode, $4.ircode, $4.val);
//
//                   }
//       ;
//
// alterd_stmt
//       : empty {
//                   cout<<"65a ";
//                   // strcpy($$.ircode,"");
//       }
//       | decl_list {
//                   cout<<"65b ";
//                   sprintf($$.ircode, "%s",$1.ircode);
//       }
//       | decl_list stmt_list {
//                   cout<<"65b ";
//                   sprintf($$.ircode, "%s\n%s",$1.ircode, $2.ircode);
//
//       }
//       ;

mainkeyword: MAIN {
				cout<<"210 ";
				 insert_gst($1.val,&tailGst,&headGst);
         strcpy(currentFun,$1.val);
         if(setPtrGst($1.val)==1){
             strcpy(PtrGst->ret_type,functionType);
         //ircode
         strcpy($$.ircode,$1.val);
         }

		}
	;

func_decl
      : fun_type_spec fun_name O_BRAC MainParamList C_BRAC body {
                  cout<<"47 ";
                  //ircode
                  funcCounter++;
                  sprintf($$.ircode, "@%s:\n%s\n%s", $2.ircode, $4.ircode, $6.ircode);
                  // emit($$.ircode);


      }
      ;

// func_body
//       : OPEN_BRAC alterd_stmt RETURN SEMICOLON CLOSE_BRAC{
//                   cout<<"65 ";
//                   //ircode
//                   sprintf($$.ircode, "%s\nreturn", $2.ircode);
//
//                   }
//       | OPEN_BRAC alterd_stmt RETURN log_or_expr SEMICOLON CLOSE_BRAC{
//                   cout<<"67 ";
//                   //ircode
//                   sprintf($$.ircode, "%s\n%s\nreturn %s", $2.ircode, $4.ircode, $4.val);
//
//                   }
//       ;


MainParamList: param_list{
					cout<<"228 ";
					// emit(strdup(":"));
                  setPtrGst(currentFun);
                   cout<<"here is headC list--";
                   printlistC(headC);
                   cout<<"--";

                  flush2(&headC);
                  //ircode
                  strcpy($$.ircode,$1.ircode);


        }
        | empty{
                  cout<<"69 ";
                  setPtrGst(currentFun);
                  PtrGst->nParam=0;

        }
        ;

fun_name : IDENTIFIER{
				cout<<"218 ";
        insert_gst($1.val,&tailGst,&headGst);

        strcpy(currentFun,$1.val);
         if(setPtrGst($1.val)==1){
             strcpy(PtrGst->ret_type,functionType);
         }
         // emit($1.val);
         //ircode
         strcpy($$.ircode,$1.val);
		}
		;

fun_type_spec:INT {
				strcpy(functionType,$1.val);

				cout<<"212 ";
				//denoted by number 1
        //ircode
        strcpy($$.ircode,$1.val);

		}
		| CHAR {

				cout<<"214 ";
        strcpy(functionType,$1.val);
				//denoted by number 2
        //ircode
        strcpy($$.ircode,$1.val);

		}
		|VOID {

				cout<<"216 ";
        strcpy(functionType,$1.val);
				//denoted by number 3
        //ircode
        strcpy($$.ircode,$1.val);

		}
		;

type_spec
      :  INT {
                  cout<<"6 ";
                  strcpy($$.val,$1.val);
                 // cout<<"---"<<$$.val<<"--";
                 //ircode
                // strcpy($$.ircode,$1.val);

      }
      | CHAR {
                  cout<<"15 ";
                 	strcpy($$.val,$1.val);
                  //ircode
                 // strcpy($$.ircode,$1.val);

      }
      ;

empty
      :  {;}
      ;

param_list
      : param_decl {
      	cout<<"224 ";
                  strcpy($$.ircode,$1.ircode);

      }
      | param_list COMMA param_decl {
                  cout<<"51 ";
                  sprintf($$.ircode,"%s\n%s", $1.ircode, $3.ircode);
      }
      ;

param_decl
      : Pdecl_spec declarator {
                  cout<<"53 ";

                 addNodeC($1.val,&tailC1,&headC1);
                 addNodeC($2.val,&tailC1,&headC1);
                 setPtrGst(currentFun);
                 flush(&headC1);
                 // emit($2.val);
                 sprintf($$.ircode,"%s %s", $1.ircode, $2.ircode);

      }
      ;
Pdecl_spec
		: type_spec{
          				strcpy($$.val,$1.val);
          				addNodeC($1.val,&tailC,&headC);
          				cout<<"226 ";
                  sprintf($$.ircode,"param %s", $1.ircode);

		}
		;

decl_spec
      : type_spec {
                cout<<"55 ";
                addNodeC($1.val,&tailC,&headC);
                strcpy($$.val,$1.val);
                strcpy(currType,$$.val);
                //ircode
               // strcpy($$.ircode,$1.ircode);


      }
      ;


body
      : compound_stmt {
                  cout<<"4 ";
                  strcpy($$.ircode,$1.ircode);
      }
      ;

compound_stmt
      : OPEN_BRAC CLOSE_BRAC{
                  cout<<"5 ";

      }

      | OPEN_BRAC stmt_list CLOSE_BRAC {
                  cout<<"17 ";
                  strcpy($$.ircode,$2.ircode);


      }

      | OPEN_BRAC decl_list CLOSE_BRAC  {
                  cout<<"85 ";
                  sprintf($$.ircode, "%s", $2.ircode);



      }
      | OPEN_BRAC decl_list stmt_list CLOSE_BRAC {
                  cout<<"87 ";
                  sprintf($$.ircode, "%s\n%s", $2.ircode, $3.ircode);


      }
      ;
decl_list
      : declare {
                  cout<<"73 ";
                  setPtrGst(currentFun);
                  printLst(PtrGst->f_tableHead);
                  //ircode
                  strcpy($$.ircode,$1.ircode);

      }
      | decl_list declare {
                  cout<<"75 ";
                  sprintf($$.ircode, "%s\n%s", $1.ircode, $2.ircode);


      }
      ;
declare
      : decl_spec init_decl_list SEMICOLON {
                  cout<<"77 ";
                  if(setPtrGst(currentFun)==1){
                    flush(&headC);

                  }
                //  printAllTables(headGst);
                //ircode
                sprintf($$.ircode, "%s %s",$1.ircode, $2.ircode);


      }
      ;
init_decl_list
      : declarators {
                cout<< "79 ";
                sprintf($$.ircode, "%s", $1.ircode);


      }
      ;
declarators
      : declarator {
                cout<< "81 ";
                addNodeC($1.val,&tailC,&headC);
                //ircode
                sprintf($$.ircode, "%s %s",currType,$1.ircode);

      }
      | declarators COMMA declarator {
                cout<<"83 ";
                addNodeC($3.val,&tailC,&headC);
                //ircode
                sprintf($$.ircode, "%s\n%s %s\n",$1.ircode, currType, $3.ircode);

      }
      ;

declarator
      : IDENTIFIER {
                cout<<"57 ";
                sprintf(buffer, "%s.%d", $1.val,funcCounter);
                strcpy($$.val,buffer);
                //  cout<<"--"<<$$.val<<"--";
                //ircode
                strcpy($$.ircode,buffer);

      }
      ;
stmt_list
      : stmt         {
                  cout<<"19 ";
                  strcpy($$.ircode,$1.ircode);

                  }
      | stmt_list stmt    {
                  cout<<"21 ";
                  sprintf($$.ircode, "%s\n%s",$1.ircode,$2.ircode);

                  }
      ;
stmt
      : compound_stmt     {
                  cout<<"23 ";
                  strcpy($$.ircode,$1.ircode);

                  }
      | expr_stmt       {
                  cout<<"25 ";
                  strcpy($$.ircode,$1.ircode);
                  }
      | iterate_stmt      {
                  cout<<"27 ";
                  strcpy($$.ircode,$1.ircode);
                  }
      | func_call_stmt  {
                  cout<<"91 ";
                  strcpy($$.ircode,$1.ircode);

         }
      | jump_stmt  {
                  cout<<"59 ";
                  strcpy($$.ircode,$1.ircode);

                  }
      | if_stmt {
                  cout<<"68 ";
                  strcpy($$.ircode,$1.ircode);

                  }
      | print_stmt{
        			cout<<"202 ";
              strcpy($$.ircode,$1.ircode);
      }
      | scan_stmt{
        			cout<<"204 ";
              strcpy($$.ircode,$1.ircode);
      }
      ;

print_stmt:
      PRINT O_BRAC IDENTIFIER C_BRAC SEMICOLON{
                cout<<"206 ";
                sprintf($$.ircode, "prnt %s.%d", $3.val,funcCounter);
                sprintf(buffer, "%s.%d", $3.val, funcCounter);
                if(lookAllTable(buffer,headGst)==0){
                  cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                  exit(0);
                }

      }
      ;
scan_stmt :
      SCAN O_BRAC IDENTIFIER C_BRAC SEMICOLON {
              cout<<"208 ";
              sprintf($$.ircode, "scan %s.%d", $3.val,funcCounter);
              sprintf(buffer, "%s.%d", $3.val, funcCounter);
              if(lookAllTable(buffer,headGst)==0){
                cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                exit(0);
              }

      }
      ;

if_stmt
      : IF O_BRAC log_or_expr C_BRAC body{
                  cout<<"72";
                  char *f ;
                  f = newLabelGen();
                  sprintf($$.ircode, "%s\nif neq %s 1 goto %s\n%s\n%s:", $3.ircode, $3.val, f, $5.ircode,f);
                  // emit($$.ircode);

      }
      | IF O_BRAC log_or_expr C_BRAC body ELSE body{
                  cout<<"74";
                  char *f, *t;
                  f = newLabelGen();
                  t = newLabelGen();

                  sprintf($$.ircode, "%s\nif neq %s 1 goto %s", $3.ircode, $3.val, f);
                  sprintf($$.ircode, "%s\n%s\ngoto %s",$$.ircode,$5.ircode,t);
                  sprintf($$.ircode, "%s\n%s:", $$.ircode,f);
                  sprintf($$.ircode, "%s\n%s\n%s:",$$.ircode,$7.ircode,t);
                  // emit($$.ircode);

      }
      ;

func_call_stmt// modified grammar to eliminate reduce/reduce conflict
      : with_return SEMICOLON{
                  cout<<"95 ";
                  strcpy($$.ircode, $1.ircode);
                  strcpy($$.val, $1.val);
      }
      | without_return SEMICOLON{
                  cout<<"95 ";
                  strcpy($$.ircode, $1.ircode);
                  strcpy($$.val, $1.val);

      }
      ;

with_return
      : IDENTIFIER EQUALS IDENTIFIER O_BRAC call_param_list C_BRAC  {
                  cout<<"99 ";
                  sprintf(buffer, "%s.%d", $1.val, funcCounter);
                  sprintf($$.ircode, "%s %s call %s %s", buffer, $2.val, $3.val, $5.ircode);
                  if(lookAllTable(buffer,headGst)==0){
                      cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                      exit(0);
                  }
                  if(lookAllTable($3.val,headGst)==0){
                      cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                      exit(0);
                  }

      }
      ;

without_return
      : IDENTIFIER O_BRAC call_param_list C_BRAC  {
                  cout<<"99 ";
                  sprintf($$.ircode, "call %s %s", $1.val, $3.ircode);
                  if(lookAllTable($1.val,headGst)==0){
                      cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                      exit(0);
                  }

      }
      ;

call_param_list
      : empty_param_list {
                  cout<<"101 ";

      }
      | non_empty_param_list {
                  cout<<"103 ";
                  sprintf($$.ircode, "%s",$1.ircode);
      }
      ;
empty_param_list
      : empty {
                  cout<<"107 ";
      }
      ;

non_empty_param_list
      : primary_expr {
                  cout<<"105 ";
                  sprintf($$.ircode, "%s",$1.val);
            }
  //ideally it should be expression instead of identifier which I will do later
      | call_param_list COMMA primary_expr
        {
                  cout<<"107 ";
                  sprintf($$.ircode, "%s %s",$1.ircode, $3.val);

        }
      ;

jump_stmt
      : CONTINUE SEMICOLON {
                  cout<<"61 ";

                  }
      | BREAK SEMICOLON  {
                  cout<<"63 ";

                  }
      | RETURN SEMICOLON {
                  cout<<"65 ";
                  strcpy($$.ircode,"return\n");
                  // emit($$.ircode);
                  }
      | EXIT SEMICOLON {
                  cout<<"655 ";
                  strcpy($$.ircode,"exit\n");
                  // emit($$.ircode);
                  }
      | RETURN log_or_expr SEMICOLON {
                  cout<<"65 ";
                  sprintf($$.ircode,"%s\nreturn %s",$2.ircode,$2.val);
                  // emit($$.ircode);
                  }

      ;
expr_stmt
      : SEMICOLON  {
                  cout<<"29 ";


      }
      | assgn_expr SEMICOLON  {
                  cout<<"31 ";
                  strcpy($$.ircode, $1.ircode);
                  strcpy($$.val, $1.val);

                }
      ;

iterate_stmt
      : WHILE O_BRAC log_or_expr C_BRAC body {
                  cout<<"35 ";
                  char *f, *t;
                  t = newLabelGen();
                  f = newLabelGen();
                  sprintf($$.ircode, "%s:\n%s\nif neq %s 1 goto %s",t, $3.ircode, $3.val, f);
                  sprintf($$.ircode, "%s\n%s",$$.ircode,$5.ircode);
                  sprintf($$.ircode, "%s\ngoto %s\n%s:", $$.ircode,t,f);
                  // emit($$.ircode);

      }
      | DO stmt WHILE O_BRAC log_or_expr C_BRAC SEMICOLON {
                  cout<<"37 ";
                  char *f, *t;
                  t = newLabelGen();
                  f = newLabelGen();
                  sprintf($$.ircode, "%s\n%s:\n%s\nif neq %s 1 goto %s",$2.ircode, t,$5.ircode, $5.val, f);
                  sprintf($$.ircode, "%s\n%s\ngoto %s",$$.ircode,$2.ircode,t);
                  sprintf($$.ircode, "%s\n%s:", $$.ircode,f);
                  // emit($$.ircode);


      }
      | FOR O_BRAC expr_stmt log_or_expr SEMICOLON C_BRAC stmt {
                  cout<<"39 ";
                  char *f, *t;
                  f = newLabelGen();
                  t = newLabelGen();
                  sprintf($$.ircode, "%s\n%s:\n%s\nif neq %s 1 goto %s",$3.ircode, t,$4.ircode, $4.val, f);
                  sprintf($$.ircode, "%s\n%s",$$.ircode,$7.ircode);
                  sprintf($$.ircode, "%s\ngoto %s\n%s:", $$.ircode,t,f);


      }
      | FOR O_BRAC expr_stmt log_or_expr SEMICOLON assgn_expr C_BRAC stmt {
                  cout<<"41 ";
                  char *f, *t;
                  f = newLabelGen();
                  t = newLabelGen();
                  sprintf($$.ircode, "%s\n%s:\n%s\nif neq %s 1 goto %s",$3.ircode, t,$4.ircode, $4.val, f);
                  sprintf($$.ircode, "%s\n%s\n%s",$$.ircode,$8.ircode,$6.ircode);
                  sprintf($$.ircode, "%s\ngoto %s\n%s:", $$.ircode,t,f);
                  // emit($$.ircode);
      }
      ;
////////////////////////////////////
primary_expr
			: IDENTIFIER{
                    cout<<"56 ";
                    sprintf(buffer, "%s.%d", $1.val, funcCounter);
                    strcpy($$.val, buffer);
                    if(lookAllTable(buffer,headGst)==0){
                      cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                      exit(0);
                  }

      }
			| NUM{
                    cout<<"58 ";
                    strcpy($$.val, $1.val);

      }
      | CHARACTER {
        		        cout<<"208 ";
                    strcpy($$.val, $1.val);

      }
			;

mul_expr
			: primary_expr{
                    cout<<"220 ";
                    strcpy($$.ircode, $1.ircode);
                    strcpy($$.val, $1.val);

      }
			| primary_expr MULTIPLY mul_expr{
                    cout<<"42 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s * %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);

      }
			| primary_expr DIVIDE mul_expr{
                    cout<<"44 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s / %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);

      }
      | primary_expr PERCENT mul_expr{
                    cout<<"46 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s %% %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);

      }
      | O_BRAC log_or_expr C_BRAC {
            				cout<<"201 ";
                    strcpy($$.ircode, $2.ircode);
                    strcpy($$.val, $2.val);

      				}
			;

add_expr
			: mul_expr{
                    cout<<"48 ";
                    strcpy($$.ircode, $1.ircode);
                    strcpy($$.val, $1.val);

      }
			| add_expr PLUS mul_expr{
                    cout<<"50 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s + %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);

      }
			| add_expr MINUS mul_expr{
                    cout<<"52 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s - %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);

      }

			;



relational_expr
			: add_expr{
                    cout<<"32 ";
                    strcpy($$.ircode, $1.ircode);
                    strcpy($$.val, $1.val);

      }
			| add_expr L_THAN relational_expr{
                    cout<<"34 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s lt %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);

      }
			| relational_expr G_THAN add_expr{
                    cout<<"36";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s gt %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);

      }
			| relational_expr L_EQ add_expr{
                    cout<<"38 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s leq %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);

      }
			| relational_expr G_EQ add_expr{
                    cout<<"40 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s geq %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);

      }
			;

equality_expr
			: relational_expr{
                    cout<<"30 ";
                    strcpy($$.ircode, $1.ircode);
                    strcpy($$.val, $1.val);

      }
			| equality_expr EQEQ relational_expr{
                    cout<<"26 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s eqeq %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);

      }
			| equality_expr NEQ relational_expr{
                    cout<<"28 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s neq %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);

      }
			;

log_and_expr
			: equality_expr{
                    cout<<"22 ";
                    strcpy($$.ircode, $1.ircode);
                    strcpy($$.val, $1.val);

      }
			| log_and_expr AND equality_expr{
                    cout<<"24 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s and %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);
      }
			;

log_or_expr
			: log_and_expr{
                    cout<<"18 ";
                    strcpy($$.ircode, $1.ircode);
                    strcpy($$.val, $1.val);
      }
			| log_or_expr OR log_and_expr{
                    cout<<"20 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s or %s", t, $1.val, $3.val);
                    sprintf($$.ircode, "%s\n%s\n%s", $1.ircode,$3.ircode, buffer);
                    strcpy($$.val, t);
      }
			;

assgn_expr// modified grammar to eliminate reduce/reduce conflict
			: IDENTIFIER EQUALS log_or_expr{
                    cout<<"200 ";
                    sprintf(buffer, "%s.%d", $1.val, funcCounter);
                    sprintf($$.ircode, "%s\n%s = %s",$3.ircode,buffer, $3.val);
                    if(lookAllTable(buffer,headGst)==0){
                      cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                      exit(0);
                    }
                    // emit($$.ircode);
      }
			;



//////////////////////////////////////


%%




int main (int argc, char* argv[]){
if(argc != 2) {
        cerr << "give argument as <output_file> " << endl;
        return 1;
        }
string outfile(argv[1]);
	//myfile.open("out.ir");
	myfile.open (outfile);
	yyparse();
	cout<<endl;
	TableDump(headGst);
	return 0;

	//./out vipin.ir<input_fibo.c
	//use make file to get out
}
