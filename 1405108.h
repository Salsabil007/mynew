#include<bits/stdc++.h>
using namespace std;
extern FILE* logfile;
class  SymbolInfo
{
public:
    string name;
    string type;
	string dtype;
	int arrsize;

	int intval;
	float floatval;

    SymbolInfo *next;
	SymbolInfo *array;
	SymbolInfo(string name,string type)
	{
		this->name = name;
		this->type = type;
		intval = -111;
		floatval = -111.11;
		next = NULL;
		arrsize = -1;
	}	

	SymbolInfo(char* name,char* type)
	{
		this->name = name;
		this->type = type;
		intval = -111;
		floatval = -111.11;
		next = NULL;
		arrsize = -1;
	}
    SymbolInfo()
    {
        next = NULL;
    }
    void setname(string s)
    {
        this->name=s;
    }
    void settype(string s)
    {
        this->type=s;
    }
    string getname()
    {
        return this->name;
    }
    string gettype()
    {
        return this->type;
    }

	void makeArray()
	{
		array = new SymbolInfo[arrsize];
		for(int i = 0; i < arrsize; i++)
		{
			array[i].type = "ID";
			array[i].dtype = dtype;
			
			if(dtype == "int") array[i].intval = -99999;
			else if(dtype == "float") array[i].floatval = -99999.0;
		}
	}

	SymbolInfo *getArrPtr(int i)
	{
		return &array[i];
	}

    ~SymbolInfo()
    {
        if(next!='\0')delete next;
    }
};
class ScopeTable
{
public:
    SymbolInfo **arr;
    ScopeTable *parent;
    int idd;
    ScopeTable()
    {
       arr = new SymbolInfo *[7] ;
        for(int i=0; i<7; i++)
        {
            arr[i]=new SymbolInfo();
            SymbolInfo *temp = new SymbolInfo;
            arr[i]->next=temp;
            temp->next='\0';
        }
    }


    SymbolInfo* look_up(string s)
    {
        int l=s.length();
        int x=l%7;
        SymbolInfo *temp=arr[x];
        int i=0;
        if(temp->next->getname()==s )
            {
                cout<<"Found At ScopeTable "<<idd<<" At position "<<x<<" "<<i<<endl;
                return temp->next;
            }
        while(temp->next->next != '\0')
        {
            if(temp->getname()==s )
            {
                cout<<"Found at ScopeTable "<<idd<<"At position "<<x<<" "<<i<<endl;
                return temp;
            }
            else temp=temp->next;
            i++;
        }
        if(temp->getname()==s )
            {
                cout<<"Found At ScopeTable "<<idd<<" at posotion "<<x<<" "<<i-1<<endl;
                return temp;
            }
        return '\0';
    }
    SymbolInfo* look_up(char* str)
    {
	return look_up((string) str);
    }

    SymbolInfo* Insert(string s,string t,string d)
    {
        if(look_up(s)=='\0')
        {
            int l=s.length();
            int x=l%7;
            SymbolInfo *temp=arr[x];
            int i=0,len=0;

            while(temp->next->next!='\0')
            {
                temp=temp->next;
                i++;
            }
            SymbolInfo *stemp=new SymbolInfo();
            stemp->setname(s);
            stemp->settype(t);
	    stemp->dtype=d;
            stemp->next=temp->next;
            temp->next=stemp;
         //   stemp->next='\0';
            cout<<"Inserted at ScopeTable "<<idd<<" At position "<<x<<" "<<i<<endl;
            return stemp;
        }
        else {cout<<"Already Exist"<<endl;return NULL;}
    }
    
        SymbolInfo* Insert(char *str,char *type,char *datatype)
    {
	Insert((string)str, (string)type, (string)datatype);
    }

	

    void Print()
    {
        for(int i=0; i<7; i++)
        {
            SymbolInfo *temp=arr[i]->next;
           // cout<<i<<" --> ";
	    fprintf(logfile,"%d ----> ",i);
            while(temp != '\0'&& temp->next!='\0')
            {
		fprintf(logfile,"< %s, %s, %s, ",temp->name,temp->type,temp->dtype);
		if(temp->dtype=="int")
		{	
			fprintf(logfile,"%d",temp->intval);	
		}
		else if(temp->dtype=="float")
		{	
			fprintf(logfile,"%lf",temp->floatval);	
		}
                temp=temp->next;
            }
		fprintf(logfile,"\n");
        }
	fprintf(logfile,"\n");
    }

    bool Delete(string s)
    {
        int l=s.length();
        int x=l%7;
        int i=0;
        SymbolInfo *temp=arr[x];
        while(temp->next->next != '\0')
        {
            if(temp->next->getname()==s)
            {
                 SymbolInfo *temp2=temp->next;
                temp->next=temp->next->next;
                cout<<"ScopeTable "<<idd<<" "<<x<<" "<<i<<endl;
                return true;
            }
            else temp=temp->next;
            i++;
        }
        cout<<"Not found"<<endl;
        return false ;
    }
    ~ScopeTable()
    {

        for(int i=0;i<7;i++)
        {
            if(arr[i] != '\0') delete arr[i];
        }
    }

};
 class SymbolTable
 {
     public:
    ScopeTable *curr;
    int id;
    SymbolTable()
    {
        id=1;
        ScopeTable *temp=new ScopeTable;
        curr=temp;
        curr->idd=id;
        curr->parent='\0';
    }
    void Enter_Scope()
    {
        id++;
        ScopeTable *temp=new ScopeTable;
        temp->parent=curr;
        curr=temp;
        curr->idd=id;
        cout<<"New ScopeTable with id "<<id<<" created"<<endl;
    }
    int Exit_Scope()
    {
        ScopeTable *temp=new ScopeTable;
        temp=curr;

        if(curr->parent != '\0') {cout<<"ScopeTable with id "<<id<<" removed"<<endl;id--;curr=curr->parent;delete temp;return 1;}
        else return 0;
    }
    SymbolInfo* Insert(string key,string type,string d)
    {
        curr->idd=id;
        return curr->Insert(key , type,d);
    }
    SymbolInfo* Insert(char* key,char* type,char* d)
    {
        Insert((string)key,(string)type,(string)d);
    }
    void Print_Curr_Scptab()
    {
        cout<<"Current ScopeTable "<<id<<endl;
        curr->Print();
    }
    void Print_All()
    {
        ScopeTable *temp=curr;
        while(temp->parent != '\0')
        {
          //  cout<<"ScopeTable# "<<temp->idd<<endl;
	    fprintf(logfile,"ScopeTable# %d\n",temp->idd);
            temp->Print();
            temp=temp->parent;

        }
     //   cout<<"ScopeTable# "<<temp->idd<<endl;
	fprintf(logfile,"ScopeTable# %d\n",temp->idd);
        temp->Print();

    }
    bool Remove(string k)
    {
         curr->Delete(k);
    }
    SymbolInfo* look_for(string s)
    {
        ScopeTable *temp=curr;
        while(temp->parent != '\0')
        {
            if(temp->look_up(s)!='\0') return temp->look_up(s);
            temp=temp->parent;

        }
        if(temp->look_up(s)!='\0') return temp->look_up(s);
        //cout<<"Not found"<<endl;
	return NULL;

    }
        SymbolInfo* look_for (char *s)
    {
	return look_for((string)s);
    }
 };




