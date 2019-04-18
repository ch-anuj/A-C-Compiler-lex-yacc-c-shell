#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <bits/stdc++.h>
#include <cstdio>
#include <sstream>
#include <regex>

using namespace std;

/*
	
	use as much code as available online so that it is clean and handles error gracefully
	use the facilities provided by c++ like vectors and hash tables and keep the code small and elegent
	don't write un-necessary functions because that will take time and induce errors
	comment the code as much as possible
*/
/////////////////////////////////////////
// char* assemblyCode;									
typedef struct A
{
	int lifeStatus;
	int index;
	
}A;
typedef struct B
{
	string varName;
	int lastUseIndex;
	int lifeStatus;	
}B;
typedef struct addresDesc
{
	int reg;
	int mem;
}addresDesc;
typedef struct nextUse
{
	int start;
	int end;
}nextUse;



int TotalRegisters=4;
int lineNoCounter = 1;									//
char buffer[100];
char cleaningBuffer[100];												//
vector< vector<string> > tokCode;
vector< vector<int> > basicBlock;
vector<string> code;
vector<vector<string>> changedUsed; //lineNo, changed, used, used.
vector<string> tempVar;
vector<A> lifeInfo;
vector<vector<B>> lineInfo;
vector< vector<int> > varLife;
std::vector<string> registers;
std::vector<string> regDesc;
std::vector<addresDesc> AdrDescTable;
std::vector<nextUse> nextUseTable;
ofstream myfile;

int caseFlag;
int lastReggen=0;
string newtempvar;

/////////////////////////////////////////
int istempvariable(string var){
	int i,flag1=0,flag2=0;
	//this is a boolean function which tells if a variable is compiler generated temporary or not
	// if the variable name contains t
	for(i=0;i<var.size();i++){
		if(var[i]=='t'){
			flag1=1;
		}
	}
	// if the variable contain a-s
	for(i=0;i<var.size();i++){
		if(var[i]>='a' && var[i]<'t'){
			flag2=1;
		}
	}
	// if the variable contain u-z
	for(i=0;i<var.size();i++){
		if(var[i]>'t' && var[i]<='z'){
			flag2=1;
		}
	}
	// if the variable contain A-Z
	for(i=0;i<var.size();i++){
		if(var[i]>='A' && var[i]<='Z'){
			flag2=1;
		}
	}
	if(flag1==1 && flag2==0)
		return 1;
	else
		return 0;

}
int isanyvariable(string var){
	int i,flag1=0,flag2=0;
	//this is a boolean function which tells if the string var is any (temp or not) variable 
	// if the variable name contains t
	for(i=0;i<var.size();i++){
		if(var[i]=='.'){
			flag1=1;
		}
	}
	return flag1;
}

bool is_number(string s)
{
	// this function checks if any string is an integer or not
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int isNonTempVariable(string var){
	// this function assumes that input is already a variable
	if(istempvariable(var))
		return 0;
	else
		return 1;
}

void printLifeInfoTable(){
	cout<<"\nvariable   lifeStatus    index\n";
	cout<<"\nsize of table="<<lifeInfo.size()<<"\n";
	for(int i=0;i<lifeInfo.size();i++){
		cout<<tempVar[i]<<" "<<lifeInfo[i].lifeStatus<<" "<<lifeInfo[i].index<<endl;
	}

	cout<<"\nend of talbe\n";
}
void printLineInfo(){
	cout<<"\nprinting line info:\n";

	cout<<"\nvar name     lifeStatus    lastUseIndex\n";
	for(int i=0;i<lineInfo.size();i++){
		for(int j=0;j<lineInfo[i].size();j++){
			cout<<lineInfo[i][j].varName<<" "<<lineInfo[i][j].lifeStatus<<" "<<lineInfo[i][j].lastUseIndex<<"    ";
		}
		cout<<endl;
	}

}
void lifetime(int startindex,int endindex){
	//startindex and endindex are for the basic block
	//this function calculates the lifetime of all the variables in a basic block (even user variables)
	//step 1: identifying all the temporary variables and storing them in a table called "tempVar"
	vector<string>::iterator itr;
	vector<string>::iterator m;	//used for finding the index in life info
	vector<A>::iterator n;
	vector<B> temp3;
	vector<int> x;
	A temp1;
	B temp2;
	int temp4;
	vector<string> shortlist;
	tempVar.clear();
	varLife.clear();
	x.clear();

	cout<<"\nentering lifetime function-\n";

	for(int i=startindex;i<=endindex;i++){
		for(int j=1;j<4;j++){
			if(isanyvariable(changedUsed[i][j])==1){
				itr = find (tempVar.begin(), tempVar.end(), changedUsed[i][j]);
				if(itr==tempVar.end()){
					tempVar.push_back(changedUsed[i][j]);
					varLife.push_back(x);	
				}
			}
			
					
			
		}
	}
	// cout<<"\nprinting all variables of the basic block\n";
	// for(int i=0;i<tempVar.size();i++){
	// 	cout<<tempVar[i]<<endl;
	// }
	for(int i=0;i<tempVar.size();i++){
		temp1.lifeStatus=0;
		temp1.index=-1;
		lifeInfo.push_back(temp1);
	}
	cout<<endl;
	//cout<<"\nprinting life info table\n";
	//printLifeInfoTable();

	for(int i=endindex;i>=startindex;i--){
		//scanning backwards
		//cout<<"\ni="<<i<<" th itration\n";
		
		shortlist.clear();

		//pushing all the temp variables in shortlist
		for(int j=1;j<4;j++){
			//checking duplicate
			if(changedUsed[i][j].compare("null")!=0){
				itr = find (shortlist.begin(), shortlist.end(), changedUsed[i][j]);
				if(itr==shortlist.end())
					shortlist.push_back(changedUsed[i][j]);
			}
			
			
		}
		// cout<<"\nprinting shortlist\n";
		// for(int d=0;d<shortlist.size();d++){
		// 	cout<<shortlist[d]<<endl;
		// }
		// printLifeInfoTable();
		// cout<<"\nattaching to index i info about X Y and Z\n";
		//attaching to index i info about X Y and Z
		temp3.clear();
		for(int j=0;j<shortlist.size();j++){
			m=find(tempVar.begin(),tempVar.end(),shortlist[j]);
			temp4=m-tempVar.begin();
			//cout<<"temp 4="<<temp4<<endl;
			n=lifeInfo.begin()+temp4;
			if(n->lifeStatus==0){

				temp2.lifeStatus=0;
				temp2.lastUseIndex=-1;
				temp2.varName=shortlist[j];
				temp3.push_back(temp2);
			}
			else{
				//cout<<"i="<<i<<"yes";
				temp2.lifeStatus=1;
				temp2.varName=shortlist[j];
				temp2.lastUseIndex=n->index;
				temp3.push_back(temp2);
			}
		}
		lineInfo.push_back(temp3);
		//printLineInfo();
		//cout<<"\nsetting X= dead in lifeinfo\n";
		//setting X = dead in LifeInfo
		if(changedUsed[i][1]!="null"){
			m=find(tempVar.begin(),tempVar.end(),changedUsed[i][1]);
			temp4=m-tempVar.begin();
			n=lifeInfo.begin()+temp4;
			n->lifeStatus=0;
			n->index=-1;
		
		}
		//printLifeInfoTable();
		//cout<<"\nsetting Y and Z to live and next use in i in lineInfo\n";
		//setting Y and Z to live and next use in i in lifeInfo
		if(changedUsed[i][2]!="null"){
			
			m=find(tempVar.begin(),tempVar.end(),changedUsed[i][2]);
			temp4=m-tempVar.begin();
			n=lifeInfo.begin()+temp4 ;
			n->lifeStatus=1;
			n->index=i;
			
		}
		if(changedUsed[i][3]!="null"){
			
			m=find(tempVar.begin(),tempVar.end(),changedUsed[i][3]);
			temp4=m-tempVar.begin();
			n=lifeInfo.begin()+temp4;
			n->lifeStatus=1;
			n->index=i;
			
		}
		//cout<<"\nhere\n";
		//printLifeInfoTable();
		

	}
	// cout<<"\ntest\n";
	// for(int i=0;i<lineInfo.size();i++){
	// 	cout<<lineInfo[i].size()<<endl;
	// }
	cout<<"\nprinting final line info table\n";
	printLineInfo();

	int z;
	z=lineInfo.size()-1+startindex;
	//cout<<"z="<<z<<endl;
	for(int k=0;k<tempVar.size();k++){
		int flag1,flag2;
		//finding the first appearence
		flag1=0;
		flag2=0;
		for(int i=lineInfo.size()-1;i>=0;i--){
			for(int j=0;j<lineInfo[i].size();j++){
				if(lineInfo[i][j].varName.compare(tempVar[k])==0 && lineInfo[i][j].lifeStatus==1){
					//cout<<tempVar[k]<<"-start="<<z-i<<endl;
					varLife[k].push_back(z-i);
					flag1=1;
				}
			}
		}
		if(flag1!=1)
			varLife[k].push_back(-1);


		//finding the last appearence
		for(int i=0;i<lineInfo.size();i++){
			for(int j=0;j<lineInfo[i].size();j++){
				if(lineInfo[i][j].varName.compare(tempVar[k])==0 && lineInfo[i][j].lifeStatus==0){
					//cout<<tempVar[k]<<"-end="<<z-i<<endl;
					varLife[k].push_back(z-i);
					flag2=1;
				}
			}
		}

		if(flag2!=1)
			varLife[k].push_back(-1);
		

	}
	nextUse ab;

	for(int i=0;i<varLife.size();i++){
		ab.start=varLife[i][0];
		ab.end=varLife[i][1];
		nextUseTable.push_back(ab);
	}
	// changing the index of -1 in nextusetable
	//don't use varlife (use nextusetable instead)
	for(int i=0;i<nextUseTable.size();i++){
		if(nextUseTable[i].start==-1){
			nextUseTable[i].start=nextUseTable[i].end;
		}
		if(nextUseTable[i].end==-1){
			nextUseTable[i].end=nextUseTable[i].start;
		}
	}

	cout<<"\nprinting the next use table\n";
	cout<<"\nvariable     startindex       endindex\n";
	for(int i=0;i<tempVar.size();i++){
		cout<<tempVar[i]<<"   "<<nextUseTable[i].start<<"   "<<nextUseTable[i].end<<endl;
	}
	// till now, the life of all the temp variables is stored in "varLife" and same info is in "nextuseTable"
	// (mapping is according to tempVar)

	//initializing the regDesc talbe
	for(int i=0;i<TotalRegisters;i++){
		regDesc.push_back("null");
	}

	//initializing address descriptor
	addresDesc adr;
	adr.reg = 0;
	adr.mem = 1;
	for(int i=0;i<tempVar.size();i++){
		AdrDescTable.push_back(adr);
	}



		



}

void print2d(vector<vector<string>> vect){
	for (int i = 0; i < vect.size(); i++) {
        for (int j = 0; j < vect[i].size(); j++)
            cout << vect[i][j] << " ";
        cout << endl;
    }
}
void print2dInt(vector<vector<int>> vect){
	for (int i = 0; i < vect.size(); i++) {
        for (int j = 0; j < vect[i].size(); j++)
            cout << vect[i][j] << " ";
        cout << endl;
    }
}
vector<string> split_string(const string& str, const string& delimiter){
    vector<string> strings;
    string::size_type pos = 0;
    string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }
    strings.push_back(str.substr(prev));
    return strings;
}

vector<string> splitStrings(string str, char dl)
{
    string word = "";

    int num = 0;
    str = str + dl;
    int l = str.size();
    vector<string> substr_list;
    for (int i = 0; i < l; i++) {
        if (str[i] != dl)
            word = word + str[i];

        else {

            if ((int)word.size() != 0)
                substr_list.push_back(word);
            word = "";
        }
    }

    return substr_list;
}
void tokenize(){
		vector<string> temp;
    regex isFun("(@)(.*)");
		regex isLabel("([a-zA-Z]+)(~)(.*)");
		regex isGoto("(goto)(.*)");
		regex isIfGoto("(if)(.*)");
		regex isEq("(=)");
		regex isPlusM("(\\+|-)");
		regex isMul("\\*|/");
		regex isOp("(gt|lt|geq|leq|and|or)");
		regex isRet("(return)");
		regex isCall("(call)");
		regex isPrint("(prnt)");
		regex isScan("(scan)");
		regex isEqCall("(= call)");
		regex isExit("(exit)");
		for (int j = 0; j < code.size(); j++) {
			auto strings = split_string(code[j], " ");
			int i = 1;

			if (regex_match(code[j],isFun)) {
				temp.push_back("1");
			}
			else	if (regex_match(code[j],isLabel)) {
				temp.push_back("2");
			}
			else	if (regex_match(code[j],isGoto)) {
				temp.push_back("3");
			}
			else	if (regex_match(code[j],isIfGoto)) {
				temp.push_back("4");
			}
			else	if (regex_search(code[j],isEq) && regex_search(code[j],isCall)) {
				temp.push_back("9");
			}
			else	if (regex_search(code[j],isEq)) {
				temp.push_back("6");
			}
			else	if (regex_search(code[j],isRet)) {
				temp.push_back("7");
			}

			else	if (regex_search(code[j],isCall)) {
				temp.push_back("8");
			}
			else	if (regex_search(code[j],isPrint)) {
				temp.push_back("10");
			}
			else	if (regex_search(code[j],isScan)) {
				temp.push_back("11");
			}
			else if (regex_search(code[j],isExit)) {
				temp.push_back("12");
			}
			else	if ( regex_search(code[j],isEq) &&
							 ( regex_search(code[j],isOp) || regex_search(code[j],isMul)
							 || regex_search(code[j],isPlusM)
								)
								) {
				temp.push_back("5");
			}
			// else	if (regex_match(code[j],isAssgn)) {
			// 	temp.push_back("6");
			// }
			for (auto itr = strings.begin(); itr != strings.end(); itr++){
					// cout << i++ << " - \"" << *itr << "\"\n";
					temp.push_back(*itr);
					}
			tokCode.push_back(temp);
			temp.clear();
		}
}

void updateChangedUsed() {
	vector<string> temp;
	regex dot("(\\.)");

	for (int j = 0; j < tokCode.size(); j++) {
		temp.clear();
		if (tokCode[j].size() - 1 == 1) { // Label, Function Label or Return or exit
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back("null");   // changed
				temp.push_back("null"); // used1.
				temp.push_back("null");   // used2
				changedUsed.push_back(temp);		
		}
		else if (tokCode[j].size() - 1 == 2) { // Prnt, Scan, Goto, CallWithoutRetrun, Return with variable
			if (tokCode[j][1] == "prnt") {
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back("null");   // changed
				temp.push_back(tokCode[j][2]); // used1.
				temp.push_back("null");   // used2
				changedUsed.push_back(temp);
			}
			else if (tokCode[j][1] == "scan") {
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back(tokCode[j][2]);   // changed
				temp.push_back("null"); // used1.
				temp.push_back("null");   // used2
				changedUsed.push_back(temp);
			}
			else if (tokCode[j][1] == "goto") {
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back("null");   // changed
				temp.push_back("null"); // used1.
				temp.push_back("null");   // used2
				changedUsed.push_back(temp);
			}
			else if (tokCode[j][1] == "return" && regex_search(tokCode[j][2],dot)  ) {
				

				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back("null");   // changed
				temp.push_back(tokCode[j][2]); // used1.
				temp.push_back("null");   // used2
				changedUsed.push_back(temp);
			}
			else if (tokCode[j][1] == "return") {
				

				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back("null");   // changed
				temp.push_back("null"); // used1.
				temp.push_back("null");   // used2
				changedUsed.push_back(temp);			}
		}
		else if (tokCode[j].size() - 1 == 3) { // Assignment
			if (regex_search(tokCode[j][3], dot)) {
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back(tokCode[j][1]);   // changed
				temp.push_back(tokCode[j][3]); // used1.
				temp.push_back("null");   // used2
				changedUsed.push_back(temp);
			}
			else{
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back(tokCode[j][1]);   // changed
				temp.push_back("null"); // used1
				temp.push_back("null");   // used2
				changedUsed.push_back(temp);
			}
		}
		else if (tokCode[j].size() - 1 == 4) { // call with return
			if (tokCode[j][3] == "call" ) {
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back(tokCode[j][1]);   // changed
				temp.push_back("null"); // used1
				temp.push_back("null");   // used2
				changedUsed.push_back(temp);
			}
		}
		else if (tokCode[j].size() - 1 == 5) { // operation
			if (regex_search(tokCode[j][3], dot) && regex_search(tokCode[j][5], dot)) {
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back(tokCode[j][1]);   // changed
				temp.push_back(tokCode[j][3]); // used1
				temp.push_back(tokCode[j][5]); // used.
				changedUsed.push_back(temp);
			}
			else if (regex_search(tokCode[j][3], dot)){
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back(tokCode[j][1]);   // changed
				temp.push_back(tokCode[j][3]); // used1
				temp.push_back("null");   // used2
				changedUsed.push_back(temp);
			}
			else if (regex_search(tokCode[j][5], dot)){
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back(tokCode[j][1]);   // changed
				temp.push_back("null");   // used1
				temp.push_back(tokCode[j][5]); // used2.
				changedUsed.push_back(temp);
			}
			else {
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back(tokCode[j][1]);   // changed
				temp.push_back("null");   // used1
				temp.push_back("null"); // used2.
				changedUsed.push_back(temp);
			}
		}
		else if (tokCode[j].size() == 7) { // ifGoto
			if (regex_search(tokCode[j][3], dot)) {
				sprintf(buffer, "%d", j);
				temp.push_back(buffer); // lineNo inserted.
				temp.push_back("null");   // changed
				temp.push_back(tokCode[j][3]); // used1
				temp.push_back("null"); // used.
				changedUsed.push_back(temp);
			}
		}
	}
}
// tokenizes the code vector into tokCode[][] vector where each
// instrunction starts with a LINENUMBER.
///////////////////////////////////////////////////////////////////

typedef struct functionArg
 {
 	string funName;
 	int numberOfArg;
 	vector<string> arglist;
 }functionArg;


void process(string* line) {
	string str1 ("");
    if(line->compare(str1) == 0){

	}
	else{
		code.push_back(*line);
	}
}



void removeWord(string* s,string* word){
	//this function removes all occurences of "word" in string "s"

	int i,length;
	size_t firstIndex;
	length = (*word).size();

	while(1){
		firstIndex = s->find(*word);
		if(firstIndex!=string::npos){
			s->erase (firstIndex,length);
		}
		else{
			break;
		}
	}
	return;

}

void clean(string& s){
	string temp("call");
	int lastindex;
	lastindex = s.find(temp);
	lastindex +=4;
	s.erase(0,lastindex);
	return;
}


void AlterCode(){
	    std::vector<string> funNameArg;			//vector to store the arguments of a function with its name
	    string tempStr("");
	    string str1("@");
	    string str2("param");
	    string str3(":");
	    string str4(" ");
	    string currentFunction;
	    size_t found,found1;
	    int v=1,count;
		   for(int i=0;i<code.size();i++){
		   	found1 = code[i].find(str1);
		   	if(found1 != string::npos){
		   		v=1;
		   		currentFunction=code[i];
		   		removeWord(&currentFunction,&str1);		//@ is removed
		   		removeWord(&currentFunction,&str3);		// : is removed
		   	}
		  	found = code[i].find(str2);
		   	if (found != string::npos) {
		   		//the word "param" is in code[i]
		   		removeWord(&code[i],&str2);		//removing the word "param"
		   		removeWord(&code[i],&str4);		//removing the extra spaces
		   		code[i]+= " = "+currentFunction+"."+to_string(v);
		   		v++;
	     	}
	     }
	    //till now all the "param" word are replaced correctly


	     str2 = "call";
	     string currentCall;
	     std::vector<string> T;
	     string Fname;
		    for(int i=0;i<code.size();i++){
		   	found1 = code[i].find(str2);
	     	if(found1 != string::npos){
	     		// the word "call is in this line"
	     		// cout<<"i="<<i;
	     		currentCall = code[i];
	     		clean(currentCall);
	     		auto string_s = splitStrings(currentCall, ' ');
	     		for (auto itr = string_s.begin(); itr != string_s.end(); itr++){
						T.push_back(*itr);
				}
				//currentCall is tokenized and in T
				Fname=T[0];
				// cout<<"\n------\n";
				string Gstring;
				Gstring = "";
				for(int k=1;k<T.size();k++){
					Gstring+=Fname+"."+to_string(k)+" = "+T[k];
					code.insert(code.begin()+i,Gstring);
					Gstring = "";
					//cout<<Fname<<"."<<k<<" = "<<T[k]<<endl;
				}

				i+=T.size()-1;
				// cout<<"\n------\n";
				T.clear();
	     	}

	     }
	   	/*
			Note: use my function for tokenization becuase,anuj's function has a problem
			(it is taken from gfg and bookmarked)
	   	*/

	   	//finally extracting the type information of the user variables
	   	unordered_map <string, int> userVariables;
	   	std::vector<string> variables;
	   	str1="int";
	   	str2="char";
	   	str3=" ";
	   	string currentVariable;
	    for(int i=0;i<code.size();i++){
	    	found1 = code[i].find(str1);
	     	if(found1 != string::npos){
	     		// the word "int" is there in the line
	     		currentVariable=code[i];
	     		removeWord(&currentVariable,&str1);		//"int" is removed
	     		removeWord(&currentFunction,&str3);		// spaces are removed
	     		userVariables[currentVariable]=1;
	     		variables.push_back(currentVariable);
	     		code.erase(code.begin()+i);
	     		i--;
	     	}
	     	found = code[i].find(str2);
	     	if(found != string::npos){
	     		// the word "char" is there in the line
	     		currentVariable=code[i];
	     		removeWord(&currentVariable,&str2);		//"char" is removed
	     		removeWord(&currentFunction,&str3);		// spaces are removed
	     		userVariables[currentVariable]=2;
	     		variables.push_back(currentVariable);
	     		code.erase(code.begin()+i);
	     		i--;
	     	}
	    }


}


void findBasicBlock(){
	vector<int> temp;
	int state;
	state=1;
	int index=0;
	int t;
	while(index<tokCode.size()){
		t=stoi(tokCode[index][0]);
		
		if(t!=3 && t!=4 && t!=1 && t!=2 && t!=7 && t!=8 && t!=9 && t!=12 && state==2){
			//middle of a basic block
			index++;
			continue;
		}
		if(t!=3 && t!=4 && t!=1 && t!=2 && t!=7 && t!=8 && t!=12 && t!=9 && state==1){
			//start of a basic block
			temp.push_back(index);
			index++;
			state=2;
			continue;
		}
		if(t==1 && state==1){
			//start of a basic block
			temp.push_back(index);
			state=2;
			index++;
			continue;
		}
		if(t==2 && state==1){
			//start of a basic block
			temp.push_back(index);
			state=2;
			index++;
			continue;
		}
		if(t==3 && state==1){
			temp.push_back(index);
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			index++;
			continue;
		}
		if(t==4 && state==1){
			temp.push_back(index);
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			index++;
			continue;
		}
		if(t==7 && state==1){
			temp.push_back(index);
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			index++;
			continue;
		}

		if(t==12 && state==1){
			temp.push_back(index);
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			index++;
			continue;
		}

		if(t==8 && state==1){
			temp.push_back(index);
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			index++;
			continue;
		}
		if(t==9 && state==1){
			temp.push_back(index);
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			index++;
			continue;
		}
		if(t==1 && state==2){
			temp.push_back(index-1);
			basicBlock.push_back(temp);
			temp.clear();
			state=1;
			continue;
		}
		if(t==2 && state==2){
			temp.push_back(index-1);
			basicBlock.push_back(temp);
			temp.clear();
			state=1;
			continue;
		}
		if(t==3 && state==2){
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			state=1;
			index++;
			continue;
		}
		if(t==4 && state==2){
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			state=1;
			index++;
			continue;
		}
		if(t==7 && state==2){
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			state=1;
			index++;
			continue;
		}
		if(t==12 && state==2){
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			state=1;
			index++;
			continue;
		}
		if(t==8 && state==2){
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			state=1;
			index++;
			continue;
		}
		if(t==9 && state==2){
			temp.push_back(index);
			basicBlock.push_back(temp);
			temp.clear();
			state=1;
			index++;
			continue;
		}
	}

}

void AlterCode1(){
	int i,j;
	int flag1=0,flag2=0;
	//flag 1 is for both call and = in i-th line
	//flat 2 is for only call in i-th line
	for(i=0;i<tokCode.size();i++){
		flag1=0;
		flag2=0;
		for(j=0;j<tokCode[i].size();j++){
			if(tokCode[i][j].compare("call")==0){

				for(int k=0;k<tokCode[i].size();k++){
					if(tokCode[i][k].compare("=")==0){
						// both call and "="  are in this line
						flag1=1;
					}
				}
				if(flag1!=1){
					flag2=1;
				}
			}
		}
		if(flag1==1){
			for(int l=5;l<tokCode[i].size();){
				tokCode[i].erase(tokCode[i].begin()+l);
			}

			//cout<<"flag1=1 for line"<<i<<endl;
		}

		if(flag2==1){
			for(int l=3;l<tokCode[i].size();){
				tokCode[i].erase(tokCode[i].begin()+l);
			}
			//cout<<"flag2=1 for line"<<i<<endl;

		}

	}
}
void emit(string s){
	cout<<endl;
	cout<<"|";
	cout<<s;
	cout<<"|";
	myfile <<s<<endl;
}
void printADandRD(){
	cout<<"\nprinting Address descriptor table\n";
	cout<<"\nvariable   reg   mem\n";
	for(int i=0;i<tempVar.size();i++){
		cout<<tempVar[i]<<" \t"<<AdrDescTable[i].reg<<" \t"<<AdrDescTable[i].mem<<endl;
	}
	cout<<"\nprinting register descriptor table\n";
	cout<<"\nregister    variable\n";
	for(int i=0;i<registers.size();i++){
		cout<<registers[i]<<" \t"<<regDesc[i]<<endl;
	}
}
int findInRegdesc(string var){
	int flag;
	//checks if the variable is in any register
	for(int i=0;i<regDesc.size();i++){
		flag=regDesc[i].compare(var);
		if(flag==0)return 1;
	}
	return 0;
}
int indexOfVar(string var){
	int i;
	for(i=0;i<tempVar.size();i++){
		if(tempVar[i].compare(var)==0){
			return i;
		}
	}
	return -1;
	cout<<"indexOfVar function failed!!";
}
int indexOfReg(string var){
	int i;
	for(i=0;i<registers.size();i++){
		if(registers[i].compare(var)==0){
			return i;
		}
	}
	return -1;
	cout<<"indexOfReg function failed!!";
}
int checkNextUse(int indexNo){
	vector<string>::iterator itr1;
	vector<nextUse>::iterator itr2;
	int x;
	//if sucess it set newtempvar to the first variable which doesnot have a next use after index= indexNo
	// and that variable is occupying some register
	// if unsucessful, it returns 0;
	// if sucessfull, it returns 1;
	for(int k=0;k<regDesc.size();k++){
		itr1=find(tempVar.begin(),tempVar.end(),regDesc[k]);
		x=itr1-tempVar.begin();
		itr2 = nextUseTable.begin()+x;
		
		if(itr2->end < indexNo){
			newtempvar = regDesc[k];
			return 1;
		}
	}
	
	return 0;

}
string getreg(int indexNo){
	int flag;
	// case 1: if any register is free,return that register
	for(int i=0;i<regDesc.size();i++){
		if(regDesc[i].compare("null")==0){
			caseFlag=1;
			lastReggen=i;
			cout<<"\ngetreg case 1 occured\n";
			cout<<"\n getreg returned "<<registers[i]<<endl;
			return registers[i];
		}
	}
	// case 2: finding a register corresponding to a variable which does not have a future use
	flag=checkNextUse(indexNo);
	if(flag==1){
		for(int i=0;i<regDesc.size();i++){
			if(regDesc[i].compare(newtempvar)==0){
				caseFlag=2;
				lastReggen=i;
				cout<<"\ngetreg case 2 occured\n";
				cout<<"\n getreg returned "<<registers[i]<<endl;
				// some book keeping before returning
				int varRemoved;
				varRemoved = indexOfVar(regDesc[i]);
				AdrDescTable[varRemoved].reg=0;
				AdrDescTable[varRemoved].mem=1;
				sprintf(buffer,"sw $%s,%s\n",registers[i].c_str(),regDesc[i].c_str());
				emit(buffer);
				return registers[i];
			}
		}
	}
	// case 3:spilling a register of the index regcycle
	caseFlag=3;
	cout<<"\nregister spilling occured\n";
	lastReggen = (lastReggen + 1)%TotalRegisters;
	sprintf(buffer,"sw $%s,%s\n",registers[lastReggen].c_str(),regDesc[lastReggen].c_str());
	emit(buffer);
	cout<<"\n getreg returned after spilling "<<registers[lastReggen]<<endl;

	//some book keeping before returning
	int varSpilled;

	varSpilled = indexOfVar(regDesc[lastReggen]);
	AdrDescTable[varSpilled].reg=0;
	AdrDescTable[varSpilled].mem=1; 

	return registers[lastReggen];
	
}

void cleanVariables(){
	cout<<"\ncleanvariables function entered:\n";

	for(int i=0;i<tempVar.size();i++){
		if(AdrDescTable[i].mem==0){
			int ka;
			for(int j=0;j<regDesc.size();j++){
						if(regDesc[j].compare(tempVar[i])==0){
							ka=j;
							break;
						}
				}
			AdrDescTable[i].mem=1;
			sprintf(cleaningBuffer,"sw $%s,%s\n",registers[ka].c_str(),tempVar[i].c_str());
			emit(cleaningBuffer);
		}
	}

}

void instructiongen(vector<string> input,int indexNo){
		string temp1("@");
		string temp_x(":");
		string temp_v;

		// instruction 1: function Label
		if(input.size()==2 && input[1].find(temp1)!=string::npos){
			
			cout<<"\nfunction label\n";
			cout<<endl;
			removeWord(&input[1],&temp1);
			temp_v = input[1];
			removeWord(&temp_v,&temp_x);
			sprintf(buffer,".globl %s\n",temp_v.c_str());
			emit(buffer);
			emit(input[1]);
			sprintf(buffer,"sub $sp,$sp,4\n");
			sprintf(buffer,"%ssw $ra,($sp)",buffer);
			emit(buffer);
			return;
		}
		// instruction 2:  A = B (simple assignment)
		// both must be variables (no constants)
		if(input.size()==4 && isanyvariable(input[1]) && isanyvariable(input[3])  && input[2].compare("=")==0  ){
			cout<<"\n A = B instruction with both variables\n";
			
				int indexA,indexB;
				indexA=indexOfVar(input[1]);
				indexB=indexOfVar(input[3]);
				//case 1 when A's recent value is in a register and B's in memory
				if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
					int ka;
					for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
					}
					// setting ka to be the index of register of A
					string regA;
					regA=registers[ka];
					sprintf(buffer, "lw $%s,%s\n",regA.c_str(),input[3].c_str());
					AdrDescTable[indexA].mem=0;
					emit(buffer);
					return;
				}	

				// case 2 when A's recent value is in memory only and B's in register
				else if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
					int kb;
					for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
					}
					// setting kb to be the index of register of B
					string regB;
					regB=registers[kb];
					string regA=getreg(indexNo);
					int indexregA=indexOfReg(regA);
					regDesc[indexregA] = input[1];
					sprintf(buffer, "move $%s,$%s\n",regA.c_str(),regB.c_str());
					AdrDescTable[indexA].reg=1;
					AdrDescTable[indexA].mem=0;
					emit(buffer);
					return;
				}
			
			//case3: when A and B both are in registers
				else if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
					int ka,kb;
					for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
					}
					// setting ka to be the index of register of A
					for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
					}
					// setting kb to be the index of register of B
					string regA,regB;
					regA=registers[ka];
					regB=registers[kb];
					sprintf(buffer,"move $%s,$%s\n",regA.c_str(),regB.c_str());
					AdrDescTable[indexA].mem=0;
					emit(buffer);
					return;

				}
				//case 4 when both A and B are only in memory
				else if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){

					cout<<"\ncase 4:\n";
					string regA;
					regA=getreg(indexNo);
					int indexregA=indexOfReg(regA);
					regDesc[indexregA]= input[1];
					sprintf(buffer,"lw $%s,%s\n",regA.c_str(),input[3].c_str());
					AdrDescTable[indexA].reg=1;
					AdrDescTable[indexA].mem=0;
					emit(buffer);
					return;

				}

		}


		// instruction 3:  A = B * C 
		// all 3 must be variables (no constants)

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) && input[4].compare("*")==0 ){

			cout<<"\nA=B*C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];
				sprintf(buffer,"mul $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"mul $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%smul $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%smul $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%smul $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%smul $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%smul $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				//cout<<"entered";
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%smul $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}
		// instruction 4:  A = constant * B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("*")==0){
			cout<<"\nA=constant * B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%smul $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%smul $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%smul $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%smul $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}


		// instruction 5:  A = B + C 
		// all 3 must be variables (no constants)

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) && input[4].compare("+")==0 ){

			cout<<"\nA=B+C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];	
				sprintf(buffer,"add $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"add $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				cout<<"entered";
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}


		// instruction 6:  A = B eqeq Constant 
		// A and B must be variables

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("eqeq")==0 ){
			cout<<"\nA = B eqeq Constant instruction \n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case1 both A and B are in memory
			// use register s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}

			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case3	A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}

			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}


		//instruction 7: if neq A constant goto L
		if(input.size()==7 && input[1].compare("if")==0 && input[2].compare("neq")==0 && isanyvariable(input[3]) && is_number(input[4]) && input[5].compare("goto")==0 ){
			
			int indexA;
			indexA=indexOfVar(input[3]);

			//case1   A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				string t("~");
				sprintf(buffer,"li $s0,%s\n",input[4].c_str());
				removeWord(&input[6],&t);
				sprintf(buffer,"%sbne $s0,$%s,%s\n",buffer,regA.c_str(),input[6].c_str());
				cleanVariables();
				emit(buffer);
				return;

			}
			//case 2 A is in memory
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[3];
				string t("~");
				sprintf(buffer,"li $s0,%s\n",input[4].c_str());
				removeWord(&input[6],&t);
				sprintf(buffer,"%sbne $s0,$%s,%s\n",buffer,regA.c_str(),input[6].c_str());
				emit(buffer);
				AdrDescTable[indexA].reg=1;
				return;


			}

		}

		//instruction 8: A = constant
		if(input.size()==4 && input[2].compare("=")==0 && isanyvariable(input[1]) && is_number(input[3]) ){
			int indexA;
			cout<<"A=constant instruction";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%s\n",regA.c_str(),input[3].c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%s\n",regA.c_str(),input[3].c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}

		}

		temp1="~";

		// instruction 9: simple Label
		if(input.size()==2 && input[1].find(temp1)!=string::npos){
			
			cout<<"\nsimple label\n";
			cout<<endl;
			removeWord(&input[1],&temp1);
			emit(input[1]);
			return;
		}

		// instruction 10: A = B lt  C
		if(input.size()==6 && input[4].compare("lt")==0 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) ){
			cout<<"\nA=B lt C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];	
				sprintf(buffer,"slt $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"slt $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				cout<<"entered";
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}






		
		// instruction 11:  A =  B  + constant 
		
		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("+")==0){
			cout<<"\nA= B + constant\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}


		// instruction 12:  goto Label 		(unconditional goto)
		temp1 = "~";
		if(input.size()==3 && input[1].compare("goto")==0 && input[2].find(temp1)!=string::npos ){
				cout<<"goto Label instruction";
				removeWord(&input[2],&temp1);
				sprintf(buffer,"j %s\n",input[2].c_str());
				cleanVariables();
				emit(buffer);
				return;
		}

		//instruction 13: return A   (A is a variable)

		if(input.size()==3 && isanyvariable(input[2]) && input[1].compare("return")==0 ){
			cout<<"\nreturn A instruction\n";
			int indexA;
			indexA=indexOfVar(input[2]);

			//case 1: A is in memory
			if(AdrDescTable[indexA].reg==0){
				sprintf(buffer,"lw $v0,%s\n",input[2].c_str());
				sprintf(buffer,"%slw $ra,($sp)\n",buffer);
				sprintf(buffer,"%saddu $sp,$sp,4\n",buffer);
				sprintf(buffer,"%sjr $ra\n",buffer);
				cleanVariables();
				emit(buffer);
				return;

			}
			//case 2: A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"lw $%s,%s\n",regA.c_str(),input[2].c_str());
				sprintf(buffer,"%smove $v0,$%s\n",buffer,regA.c_str());
				sprintf(buffer,"%slw $ra,($sp)\n",buffer);
				sprintf(buffer,"%saddu $sp,$sp,4\n",buffer);
				sprintf(buffer,"%sjr $ra\n",buffer);
				cleanVariables();
				emit(buffer);
				return;



			}
		}

		// Instruction 14 return constant
		if(input.size()==3 && is_number(input[2]) && input[1].compare("return")==0 ){
			cout<<"\nreturn constant instruction\n";
			sprintf(buffer,"li $v0,%s\n",input[2].c_str());
			sprintf(buffer,"%slw $ra,($sp)\n",buffer);
			sprintf(buffer,"%saddu $sp,$sp,4\n",buffer);
			sprintf(buffer,"%sjr $ra\n",buffer);
			emit(buffer);
			return;
		}	

		// Instruction 15 return   (without arguments)

		if(input.size()==2  && input[1].compare("return")==0 ){
			cout<<"\nreturn without arguments\n";
			sprintf(buffer,"lw $ra,($sp)\n");
			sprintf(buffer,"%saddu $sp,$sp,4\n",buffer);
			sprintf(buffer,"%sjr $ra\n",buffer);
			emit(buffer);
			return;
		}

		// Instruction 16 call foo  (without assignment)
		if(input.size()==3 && input[1].compare("call")==0){
			cout<<"call foo without assignment"<<endl;
			sprintf(buffer,"jal %s\n",input[2].c_str());
			cleanVariables();
			emit(buffer);
			return;
		}
		//Instruction 17 A = call foo  (A is variable)
		if(input.size()==5 && isanyvariable(input[1]) && input[3].compare("call")==0  ){
			cout<<"call foo with assignment"<<endl;
			int indexA;
			indexA=indexOfVar(input[1]);
			//case 1 A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];

				sprintf(buffer,"jal %s\n",input[4].c_str());
				sprintf(buffer,"%smove $%s,$v0\n",buffer,regA.c_str());
				AdrDescTable[indexA].mem=0;
				cleanVariables();
				emit(buffer);
				return;

			}
			//case 2: A is in memory
			if(AdrDescTable[indexA].reg==0){
				
				sprintf(buffer,"jal %s\n",input[4].c_str());
				sprintf(buffer,"%ssw $v0,%s\n",buffer,input[1].c_str());
				AdrDescTable[indexA].mem=1;
				cleanVariables();
				emit(buffer);
				return;
			}


		}

		//Instruction 18 prnt A  (A is variable)
		if(input.size()==3 && input[1].compare("prnt")==0 && isanyvariable(input[2]) ){
			cout<<"prnt variable instruction"<<endl;
			int indexA;
			indexA = indexOfVar(input[2]);
			//case 1 A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[2])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $v0,1\n");
				sprintf(buffer,"%smove $a0,$%s\n",buffer,regA.c_str());
				sprintf(buffer,"%ssyscall\n",buffer);
				emit(buffer);
				return;

			}
			//case 2 A is in memory
			if(AdrDescTable[indexA].reg==0){
				sprintf(buffer,"li $v0,1\n");
				sprintf(buffer,"%slw $a0,%s\n",buffer,input[2].c_str());
				sprintf(buffer,"%ssyscall\n",buffer);
				emit(buffer);
				return;

			}

			
		}

		// Instruction 19 : prnt constant
		if(input.size()==3 && input[1].compare("prnt")==0 && is_number(input[2]) ){
			sprintf(buffer,"li $v0,1\n");
			sprintf(buffer,"%sli $a0,%s\n",buffer,input[2].c_str());
			sprintf(buffer,"%ssyscall\n",buffer);
			emit(buffer);
			return;
		}

		// Instruction 20: scan A
		if(input.size()==3 && input[1].compare("scan")==0 && isanyvariable(input[2])){
			
			int indexA;
			indexA = indexOfVar(input[2]);
			// if A is in register,free the register
			if(AdrDescTable[indexA].reg==1){
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[2])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				registers[ka]="null";
				AdrDescTable[indexA].reg=0;

			}
			sprintf(buffer,"li $v0,5\n");
			sprintf(buffer,"%ssyscall",buffer);
			sprintf(buffer,"%ssw $v0,%s\n",buffer,input[2].c_str());
			AdrDescTable[indexA].mem=1;

		}

		//instruction 21  exit
		if(input.size()==2 && input[1].compare("exit")==0){
			sprintf(buffer,"li $v0,10\n");
			sprintf(buffer,"%ssyscall\n",buffer);
			cleanVariables();
			emit(buffer);
			return;
		}

		//instruction 22 A = B*constant
		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("*")==0){
			cout<<"\nA= B * constant\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%smul $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%smul $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%smul $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%smul $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//Instruction 23: A = constant * constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("*")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = c1*c2;

			int indexA;
			cout<<"A=constant * constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}

		// instruction 24:  A = constant + B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("+")==0){
			cout<<"\nA=constant + B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sadd $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//Instruction 25: A = constant + constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("+")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = c1+c2;

			int indexA;
			cout<<"A=constant + constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}

		// instruction 26:  A = B - C 
		// all 3 must be variables (no constants)

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) && input[4].compare("-")==0 ){

			cout<<"\nA=B-C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];	
				sprintf(buffer,"sub $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"sub $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}

		//instruction 27 A = B-constant
		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("-")==0){
			cout<<"\nA= B - constant\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		// instruction 28:  A = constant - B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("-")==0){
			cout<<"\nA=constant - B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%ssub $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//instruction 29: A = constant - constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("-")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = c1-c2;

			int indexA;
			cout<<"A=constant - constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}

		// instruction 30:  A = B / C 
		// all 3 must be variables (no constants)

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) && input[4].compare("/")==0 ){

			cout<<"\nA=B/C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];	
				sprintf(buffer,"div $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"div $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}

		//instruction 31 A = B/constant
		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("/")==0){
			cout<<"\nA= B / constant\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		// instruction 32:  A = constant / B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("/")==0){
			cout<<"\nA=constant / B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sdiv $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//instruction 33: A = constant / constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("/")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = c1/c2;

			int indexA;
			cout<<"A=constant / constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}

		// instruction 34:  A = B gt C 
		// all 3 must be variables (no constants)

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) && input[4].compare("gt")==0 ){

			cout<<"\nA=B gt C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];	
				sprintf(buffer,"sgt $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"sgt $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}


		//instruction 35 A = B gt constant
		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("gt")==0){
			cout<<"\nA= B gt constant\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}


		// instruction 36:  A = constant gt B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("gt")==0){
			cout<<"\nA=constant gt B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%ssgt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//instruction 37: A = constant gt constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("gt")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = c1>c2;

			int indexA;
			cout<<"A=constant gt constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}


		//instruction 39 A = B lt constant
		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("lt")==0){
			cout<<"\nA= B lt constant\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}


		// instruction 40:  A = constant lt B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("lt")==0){
			cout<<"\nA=constant lt B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sslt $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//instruction 41: A = constant lt constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("lt")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = c1<c2;

			int indexA;
			cout<<"A=constant lt constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}

		// instruction 42:  A = B eqeq C 
		// all 3 must be variables (no constants)

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) && input[4].compare("eqeq")==0 ){

			cout<<"\nA=B eqeq C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];	
				sprintf(buffer,"seq $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"seq $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}

		// instruction 43:  A = constant eqeq B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("eqeq")==0){
			cout<<"\nA=constant eqeq B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sseq $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//instruction 44: A = constant eqeq constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("eqeq")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = c1==c2;

			int indexA;
			cout<<"A=constant eqeq constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}


		// instruction 45:  A = B neq C 
		// all 3 must be variables (no constants)

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) && input[4].compare("neq")==0 ){

			cout<<"\nA=B neq C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];	
				sprintf(buffer,"sne $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"sne $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}


		//instruction 46 A = B neq constant
		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("neq")==0){
			cout<<"\nA= B neq constant\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}


		// instruction 47:  A = constant neq B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("neq")==0){
			cout<<"\nA=constant neq B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%ssne $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//instruction 48: A = constant neq constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("neq")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = (c1!=c2);

			int indexA;
			cout<<"A=constant neq constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}


		// instruction 49:  A = B geq C 
		// all 3 must be variables (no constants)

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) && input[4].compare("geq")==0 ){

			cout<<"\nA=B geq C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];	
				sprintf(buffer,"sge $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"sge $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}


		//instruction 50 A = B geq constant
		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("geq")==0){
			cout<<"\nA= B geq constant\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}


		// instruction 51:  A = constant geq B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("geq")==0){
			cout<<"\nA=constant geq B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%ssge $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//instruction 52: A = constant geq constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("geq")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = c1>=c2;

			int indexA;
			cout<<"A=constant geq constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}

		// instruction 53:  A = B leq C 
		// all 3 must be variables (no constants)

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) && input[4].compare("leq")==0 ){

			cout<<"\nA=B leq C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];	
				sprintf(buffer,"sle $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"sle $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}


		//instruction 54 A = B leq constant
		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("leq")==0){
			cout<<"\nA= B leq constant\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}


		// instruction 55:  A = constant leq B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("leq")==0){
			cout<<"\nA=constant leq B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%ssle $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//instruction 56: A = constant leq constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("leq")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = c1<=c2;

			int indexA;
			cout<<"A=constant leq constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}


		// instruction 57:  A = B and C 
		// all 3 must be variables (no constants)

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) && input[4].compare("and")==0 ){

			cout<<"\nA=B and C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];	
				sprintf(buffer,"and $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"and $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sand $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sand $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sand $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sand $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sand $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sand $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}


		//instruction 58 A = B and constant
		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("and")==0){
			cout<<"\nA= B and constant\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sand $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sand $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sand $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sand $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}


		// instruction 59:  A = constant and B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("and")==0){
			cout<<"\nA=constant and B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sand $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sand $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sand $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sand $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//instruction 60: A = constant and constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("and")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = c1&&c2;

			int indexA;
			cout<<"A=constant and constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}

		// instruction 61:  A = B or C 
		// all 3 must be variables (no constants)

		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && isanyvariable(input[5]) && input[4].compare("or")==0 ){

			cout<<"\nA=B or C instruction with all variables\n";
			int indexA,indexB,indexC;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			indexC=indexOfVar(input[5]);
			//case 1  r r r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int ka,kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
					}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regA=registers[ka];
				regB=registers[kb];
				regC=registers[kc];	
				sprintf(buffer,"or $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}

			//case 2: m r r
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==1){
				int kb,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B
				for(int i=0;i<regDesc.size();i++){
					if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
					}
				}
				// setting kc to be the index of register of C
				string regA,regB,regC;
				regB=registers[kb];
				regC=registers[kc];
				regA = getreg(indexNo);
				int indexregA = indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"or $%s,$%s,$%s\n",regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;
			}

			//case3 m m m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				// cout<<"entered";
				string regA,regB,regC;
				int indexregA,indexregB,indexregC;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sor $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case4 r m m

			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregB,indexregC;

				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sor $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;	
				AdrDescTable[indexB].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

			//case 5  r m r
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int indexregB;
				int ka,kc;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C

				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sor $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 6  r r m
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				string regA,regB,regC;
				int indexregC;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				regC=getreg(indexNo);

				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sor $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}
			//case 7  m m r   
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0 && AdrDescTable[indexC].reg==1){
				string regA,regB,regC;
				int kc;
				int indexregA,indexregB;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kc=i;
							break;
						}
				}
				// setting kc to be the index of register of C
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				regC=registers[kc];
				sprintf(buffer,"lw $%s,%s\n",regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sor $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}

			//case 8: m r m
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1 && AdrDescTable[indexC].reg==0){
				
				string regA,regB,regC;
				int kb;
				int indexregA,indexregC;

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				//cout<<"kb="<<kb;
				// setting kb to be the index of register of B
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regC=getreg(indexNo);
				indexregC=indexOfReg(regC);
				regDesc[indexregC]=input[5];
				regB=registers[kb];

				//cout<<"regB="<<regB;
				
				
				sprintf(buffer,"lw $%s,%s\n",regC.c_str(),input[5].c_str());
				sprintf(buffer,"%sor $%s,$%s,$%s\n",buffer,regA.c_str(),regB.c_str(),regC.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexC].reg=1;
				emit(buffer);
				return;
			}

		}


		//instruction 62 A = B or constant
		if(input.size()==6 && isanyvariable(input[1]) && isanyvariable(input[3]) && is_number(input[5]) && input[4].compare("or")==0){
			cout<<"\nA= B or constant\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[3]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sor $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[3];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[3].c_str());
				sprintf(buffer,"%sor $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sor $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[3])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[5].c_str());
				sprintf(buffer,"%sor $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}


		// instruction 63:  A = constant or B 
		
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && isanyvariable(input[5]) && input[4].compare("or")==0){
			cout<<"\nA=constant or B\n";
			int indexA,indexB;
			indexA=indexOfVar(input[1]);
			indexB=indexOfVar(input[5]);
			//case 1 A is in memory and B is in memory
			//use register $s0 here
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregA,indexregB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sor $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;
			}
			//case 2: A is in register and B is in memory
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==0){
				string regA,regB;
				int indexregB;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A


				regA=registers[ka];
				regB=getreg(indexNo);
				indexregB=indexOfReg(regB);
				regDesc[indexregB]=input[5];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%slw $%s,%s\n",buffer,regB.c_str(),input[5].c_str());
				sprintf(buffer,"%sor $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexB].reg=1;
				emit(buffer);
				return;

			}
			//case 3: A is in memory and B is in register
			if(AdrDescTable[indexA].reg==0 && AdrDescTable[indexB].reg==1){
				// cout<<"entered";
				string regA,regB;
				int indexregA;
				int kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B


				regB=registers[kb];
				// cout<<"regB="<<regB;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sor $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}
			//case 4: Both A and B are in registers
			if(AdrDescTable[indexA].reg==1 && AdrDescTable[indexB].reg==1){
				string regA,regB;
				int ka,kb;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A

				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[5])==0){
							kb=i;
							break;
						}
				}
				// setting kb to be the index of register of B

				regA=registers[ka];
				regB=registers[kb];
				sprintf(buffer,"li $s0,%s\n",input[3].c_str());
				sprintf(buffer,"%sor $%s,$%s,$s0\n",buffer,regA.c_str(),regB.c_str());
				sprintf(buffer,"%ssge $%s,$%s,1\n",buffer,regA.c_str(),regA.c_str());
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			

		}

		//instruction 64: A = constant or constant
		if(input.size()==6 && isanyvariable(input[1]) && is_number(input[3]) && is_number(input[5]) && input[4].compare("or")==0 ){
			int result;
			int c1,c2;
			c1 = stoi(input[3]);
			c2 = stoi(input[5]);
			result = c1||c2;

			int indexA;
			cout<<"A=constant or constant";
			indexA=indexOfVar(input[1]);
			//case1 if A is in register
			if(AdrDescTable[indexA].reg==1){
				string regA;
				int ka;
				for(int i=0;i<regDesc.size();i++){
						if(regDesc[i].compare(input[1])==0){
							ka=i;
							break;
						}
				}
				// setting ka to be the index of register of A
				regA=registers[ka];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				emit(buffer);
				return;
			}
			//case 2 if A is in memory only
			if(AdrDescTable[indexA].reg==0){
				string regA;
				int indexregA;
				regA=getreg(indexNo);
				indexregA=indexOfReg(regA);
				regDesc[indexregA]=input[1];
				sprintf(buffer,"li $%s,%d\n",regA.c_str(),result);
				AdrDescTable[indexA].mem=0;
				AdrDescTable[indexA].reg=1;
				emit(buffer);
				return;

			}


		}




		// invalidity statement at the end
		cout<<"\n\n\n*********\n\n\n\n";
		cout<<"invalid instruction:<";
		for(int i=1;i<input.size();i++){
			cout<<input[i]<<" ";
		}
		cout<<">"<<endl;
		cout<<"\n\n\n*********\n\n\n\n";



}

void codegen(int sIndex,int eIndex){
	//this function generates code for a basic block given by
	// sIndex,eIndex
	emit("\n#start of a basic block\n");
	lifetime(sIndex,eIndex);
	//allocating resources
	for(int i=sIndex;i<=eIndex;i++){
		instructiongen(tokCode[i],i);
	}

	emit("\n#putting dirty variables back into memory\n");
	cout<<"\nputting dirty variables back into memory\n";
	printADandRD();
	//putting the dirty variables back into memory
	
	for(int i=0;i<tempVar.size();i++){
		if(AdrDescTable[i].mem==0){
			int ka;
			for(int j=0;j<regDesc.size();j++){
						if(regDesc[j].compare(tempVar[i])==0){
							ka=j;
							break;
						}
				}
			AdrDescTable[i].mem=1;
			sprintf(buffer,"sw $%s,%s\n",registers[ka].c_str(),tempVar[i].c_str());
			emit(buffer);
		}
	}

	cout<<"\n------------end for codegen function-----------\n";
	//emptying the resources
	tempVar.clear();
	lifeInfo.clear();
	nextUseTable.clear();
	regDesc.clear();
	AdrDescTable.clear();
	varLife.clear();
	lineInfo.clear();

}

int main(int argc, char* argv[]) {
    string line;
    
    //myfile.open ("vipin.asm");
    if(argc != 3) {
        cerr << "give arguments as <input_file> <output_file> " << endl;
        return 1;
        }
    string filename(argv[1]);
    string outfile(argv[2]);
    myfile.open(outfile);
  //  cout << "* trying to open and read: " << filename << endl;
    ifstream f (argv[1]);
    if (!f.is_open())
        perror(("error while opening file " + filename).c_str());

    while(getline(f, line)) {
        process(&line);
        }

    if (f.bad())
        perror(("error while reading file " + filename).c_str());
    f.close();


    // cout<<"finally printing the vector:"<<endl;
    // for(int i=0;i<code.size();i++){
    // 	cout<<code[i]<<endl;
    // }

    	// 4 registers for holding temp variables 
    	// note that there may be other registers also (but fixed use for them)
		registers.push_back("t0");
		registers.push_back("t1");
		registers.push_back("t2");
		registers.push_back("t3");


		AlterCode();

		tokenize();
		// the input is tokenized and it is in tokCode


		findBasicBlock();		
		//basic block are found
		
		AlterCode1();
		//changing the code a little bit

		print2d(tokCode);
		cout<<endl<<endl;


		


		updateChangedUsed();
		cout<<endl<<endl;
		print2d(changedUsed);

		std::vector<string> programVariables; 	// to store all the program variables
		vector<string>::iterator m_itr;
		for(int i=0;i<changedUsed.size();i++){
			for(int j=1;j<4;j++){
				if(isanyvariable(changedUsed[i][j])==1){
					m_itr = find (programVariables.begin(), programVariables.end(), changedUsed[i][j]);
					if(m_itr==programVariables.end()){
						programVariables.push_back(changedUsed[i][j]);	
					}
				}
			
					
			
			}
		}

		cout<<"<printing all program variables>"<<endl;
		for(int i=0;i<programVariables.size();i++){
			cout<<programVariables[i]<<endl;
		}

		cout<<endl;

		//printing the basic blocks
		for(int i=0;i<basicBlock.size();i++){
			for(int j=0;j<basicBlock[i].size();j++){
				cout<<basicBlock[i][j]<<" ";
			}
			cout<<endl;
		}
				
		//lifetime function is critical to setting up the resourses
		//for the particular basic block
		//it also initializes the AdresDescTable and RegDescTable

		// creating the data section
		emit("\n.data\n");
		for(int i=0;i<programVariables.size();i++){
			sprintf(buffer,"%s:\t.word\t0\n",programVariables[i].c_str());
			emit(buffer);
		}

	
		// finaly generating the code!!
		emit("\n.text\n");
		for(int i=0;i<basicBlock.size();i++){
			codegen(basicBlock[i][0],basicBlock[i][1]);
		}
		
		cout<<endl;
		
		
		


  return 0;
}


//convention: return value of every function (if applicable) will be in v0
//there will be 3 cases in "return"  instruction
//1. return;
//2. return constant
//3. return A  (variable)

//similarly there will be 3-4 cases in the call instruction
//function label instruction is already done

//while running,don't use make
//it will enable you to choose your input.ir and output.asm file names
//apply the keyword "new" for the shell script