#include<iostream>
#include<string>
#include<conio.h>
#include<stdlib.h>
#include<fstream>
#include<string.h>
#define V 5
#define INF 99999
using namespace std;

/*	
*Variables that are used for delivery and waiting list
*/
int am=0, vpizzacol=0, damt=0;

int wprice[100];
char wname[100][100];
int wq[100];
int wpr=-1;
int wpf=0;


int dprice[100];
char dname[100][100];
int dloc[100];
int dq[100];
int dpr=-1;
int dpf=0;

/*
*These variables store the current location and next delivery location.
*/
int cdloc=0;
int lo;

/*
*Methods of this class are used to delete the served item from the list.
*/
class DequeFront{
public:
	void dequeFrontW(){
	wpf++;
	}

void dequeFrontD(){
	dpf++;
	}
};

/*
*This is a standard BTree Node
*/
class Btn{
public:
    int *key;
	int *price;
	char **name;
	int *avail;  
    int t;      
    Btn **c; 
    int n;   
    bool l;
 
public:
/*
*Displays the content stored at the Index i.
*/
void displayContents(int i){
	cout<<"\nKey:"<<"\t"<<key[i];
	cout<<"\nPrice:"<<"\t"<<price[i];
	cout<<"\nName:"<<"\t"<<name[i];
	cout<<"\nAvailable:"<<"\t"<<avail[i];
}

/*
*Adds an item to a Waiter's List.
*/
void addToListW(int i, int q){
	strcpy(wname[++wpr],name[i]);
	wq[wpr]=q;
	wprice[wpr]=wq[wpr]*price[i];
	cout<<"\n"<<wname[wpr]<<" Added.";
}

/*
*Adds an Item to the delivery list.
*/
void addToListD(int i,int q){
	dpr++;
	strcpy(dname[dpr],name[i]);
	dq[dpr]=q;
	dprice[dpr]=dq[dpr]*price[i];
	cout<<"\nEnter Where to deliver:";
	cin>>dloc[dpr];
}
    
/*
*The constructor of the Class Btn.
*Allocates Memory and specifies if it is a leaf or not.
*/
Btn(int t1, bool leaf1){
    t = t1;
    l = leaf1; 
    key = new int[2*t-1];
    price = new int[2*t-1];
    avail = new int[2*t-1];
    name= new char *[2*t-1];
    for(int i=0;i<2*t-1;i++){
    	name[i]=new char [20];
	}
    c = new Btn *[2*t];
	n = 0;
	}
    
/*
*Traverses the Whole tree.
*/
	
void treeTrav(){
	int i;
    for (i = 0; i < n; i++){
		if (l == false)
            c[i]->treeTrav();
        cout<<"\n"<<key[i]<<"\t"<<name[i]<<"\t\t"<<price[i]<<"\t"<<avail[i]<<"\n";
    }
    if (l == false)
        c[i]->treeTrav();
	}
	
/*
*Traverses the Tree.
*It prints to a File rather than the Output Stream.
*/
void printToFile(){
	static ofstream fout("menu.txt");
	int i;
    for (i = 0; i < n; i++){
		if (l == false)
            c[i]->printToFile();
        fout<<key[i]<<" "<<name[i]<<" "<<price[i]<<" "<<avail[i]<<"\n";
    }
    if (l == false)
        c[i]->printToFile();
	}

void splitChild(int i, Btn *y){
    Btn *z = new Btn(y->t, y->l);
    z->n = t - 1;
    for (int j = 0; j < t-1; j++){
    	z->key[j] = y->key[j+t];
    	strcpy(z->name[j],y->name[j+t]);
    	z->price[j] = y->price[j+t];
    	z->avail[j] = y->avail[j+t];
	}
    if (y->l == false){
        for (int j = 0; j < t; j++)
            z->c[j] = y->c[j+t];
    }
    y->n = t - 1;
    for (int j = n; j >= i+1; j--)
        c[j+1] = c[j];
    c[i+1] = z;
    for (int j = n-1; j >= i; j--){
    	key[j+1] = key[j];
    	strcpy(name[j+1],name[j]);
    	price[j+1] = price[j];
    	avail[j+1] = avail[j];
	}
        
    key[i] = y->key[t-1];
    strcpy(name[i],y->name[t-1]);
    price[i] = y->price[t-1];
    avail[i] = y->avail[t-1];
    n = n + 1;
}
    
int findKey(int k){
    int x=0;
    while (x<n && key[x] < k)
        ++x;
    return x;
	}

Btn *searchKey(int k, int &in){
	int i = 0;
    while (i < n && k > key[i])
        i++;
    if (key[i]==k){
    	//cout<<"\nFound";
    	in=i;
    	return this;    	
	}
    if (l == true)
        return NULL;
    return c[i]->searchKey(k,in);
}
    
void insertNonFull(int k, char *na, int pr, int av){ 
    int i = n-1;
    if (l == true){
        while (i >= 0 && key[i] > k){
            key[i+1] = key[i];
            strcpy(name[i+1],name[i]);
            price[i+1] = price[i];
            avail[i+1] = avail[i];
            i--;
        }
        key[i+1] = k;
        strcpy(name[i+1],na);
        price[i+1] = pr;
        avail[i+1] = av;
        n = n+1;
    }
    else{
    	while (i >= 0 && key[i] > k)
            i--;
        if (c[i+1]->n == 2*t-1){
        	splitChild(i+1, c[i+1]);
        	if (key[i+1] < k)
                i++;
        }
        c[i+1]->insertNonFull(k,na,pr,av);
    }
}
    
void removeKey(int k){
    int x = findKey(k);
    if (x < n && key[x] == k){
    	if (l)
            removeFromLeaf(x);
        else
            removeFromNonLeaf(x);
    }
    else{ 
        if (l){
            cout << "\nData not Found.";
            return;
        }
 		bool flag = ( (x==n)? true : false );
 
        if (c[x]->n < t)
            fill(x);
 		
		if (flag && x > n)
            c[x-1]->removeKey(k);
        
		else
            c[x]->removeKey(k);
    }
    return;
}
    
    
    
void borrowFromPrev(int x){
	Btn *child=c[x];
    Btn *sibl=c[x-1];
    for (int i=child->n-1; i>=0; --i){
    	child->key[i+1] = child->key[i];
    	strcpy(child->name[i+1],child->name[i]);
    	child->price[i+1] = child->price[i];
    	child->avail[i+1] = child->avail[i];
	}
        
    if (!child->l){
        for(int i=child->n; i>=0; --i)
            child->c[i+1] = child->c[i];
    }
    child->key[0] = key[x-1];
    strcpy(child->name[0],name[x-1]);
    child->price[0] = price[x-1];
    child->avail[0] = avail[x-1];
    if (!l)
        child->c[0] = sibl->c[sibl->n];
    key[x-1] = sibl->key[sibl->n-1];
    strcpy(name[x-1],sibl->name[sibl->n-1]);
    price[x-1] = sibl->price[sibl->n-1];
    avail[x-1] = sibl->avail[sibl->n-1];
    child->n += 1;
    sibl->n -= 1;
	return;
}
    
    
void removeFromNonLeaf(int x){
    int k = key[x];
 	if (c[x]->n >= t){
        int pred = getPre(x);
        key[x] = pred;
        c[x]->removeKey(pred);
    }
 
    else if(c[x+1]->n >= t){
        int succ = getSuc(x);
        key[x] = succ;
        c[x+1]->removeKey(succ);
    }
 
    else{
        merge(x);
        c[x]->removeKey(k);
    }
    return;
}
    
    
    
int getPre(int x){
    Btn *cur=c[x];
    while (!cur->l)
        cur = cur->c[cur->n];
        return cur->key[cur->n-1];
	}
	void merge(int x){
    Btn *child = c[x];
    Btn *sibl = c[x+1];
    child->key[t-1] = key[x];
    for (int i=0; i<sibl->n; ++i){
    	child->key[i+t] = sibl->key[i];
    	strcpy(child->name[i+t],sibl->name[i]);
    	child->price[i+t] = sibl->price[i];
    	child->avail[i+t] = sibl->avail[i];
	}
    if (!child->l){
        for(int i=0; i<=sibl->n; ++i){
        	child->c[i+t] = sibl->c[i];
		}
    }
    for (int i=x+1; i<n; ++i){
    	key[i-1] = key[i];
    	strcpy(name[i-1], name[i]);
    	price[i-1] = price[i];
    	avail[i-1] = avail[i];
	}
    
    for (int i=x+2; i<=n; ++i)
        c[i-1] = c[i];
    child->n += sibl->n+1;
    n--; 
    delete(sibl);
    return;
}
    
    
void removeFromLeaf(int x){
    for (int i=x+1; i<n; ++i){
    	key[i-1] = key[i];
    	strcpy(name[i-1], name[i]);
    	price[i-1] = price[i];
    	avail[i-1] = avail[i];
	}
    n--;
	return;
}
    
	
void fill(int x){
    if(x!=0 && c[x-1]->n>=t)
        borrowFromPrev(x);
 
    else if(x!=n && c[x+1]->n>=t)
        borrowFromNext(x);
     
    else{
        if (x != n)
            merge(x);
        else
            merge(x-1);
    }
    return;
}
    
    
int getSuc(int x){
    Btn *cur = c[x+1];
    while (!cur->l)
        cur = cur->c[0];
    return cur->key[0];
}
    
void borrowFromNext(int x){
	Btn *child=c[x];
    Btn *sibl=c[x+1];
    child->key[(child->n)] = key[x];
    if (!(child->l))
        child->c[(child->n)+1] = sibl->c[0];
    key[x] = sibl->key[0];
    for(int i=1; i<sibl->n; ++i){
    	sibl->key[i-1] = sibl->key[i];
    	strcpy(sibl->name[i-1],sibl->name[i]);
    	sibl->price[i-1] = sibl->price[i];
    	sibl->avail[i-1] = sibl->avail[i];
	}
    if (!sibl->l){
        for(int i=1; i<=sibl->n; ++i)
            sibl->c[i-1] = sibl->c[i];
    }
    child->n += 1;
    sibl->n -= 1; 
    return;
}
/*
*It is declared as a friend so that Bt can access its methods.
*/
friend class Bt;
};

/*
*A class for BTree.
*It servers as an interface to access the contents of the BTree.
*/ 
class Bt{
    Btn *root; 
    int t;
public:
	
/*
*The constructor of the class.
*/
Bt(int tt){
        root=NULL;
        t=tt;
    }
/*
*All these methods acts as an Interface to access the methods of the Class Btn.
*/
void treeTrav(){
        if (root!=NULL){
        	root->treeTrav();
		}
    }

void printToFile(){
        if (root!=NULL){
        	root->printToFile();
		}
    }
    
void addMenu(){
	ifstream fin("menu.txt");
	int te,tpr,tav;
	char tna[20];
	while(fin){
		fin>>te>>tna>>tpr>>tav;
		insertKey(te,tna,tpr,tav);
	}
	fin.close();
}
 
Btn* searchKey(int k,int &in){
        return (root==NULL)? NULL : root->searchKey(k,in);
    }
    

/*
*This method updates the contents by two ways.
*It deletes the specific item from the tree and then inserts it again with updated values.
*/
void update(int k){
	int in;
	Btn *node=searchKey(k,in);
	char nam[20];
	strcpy(nam,node->name[in]);
	removeKey(k);
	cout<<"Enter the Price: Rs.";
	int pr,av;
	cin>>pr;
	cout<<"\nNumber of Items: ";
	cin>>av;
	insertKey(k,nam,pr,av);
}

void updateMisc(int k, int pu){
	int in;
	Btn *node=searchKey(k,in);
	char nam[20];
	strcpy(nam,node->name[in]);
	int pri=node->price[in];
	int ava=node->avail[in];
	removeKey(k);
	insertKey(k,nam,pri,ava-pu);
}


void updateKitchen(int k){
	int in;
	Btn *node=searchKey(k,in);
	char nam[20];
	strcpy(nam,node->name[in]);
	int pri=node->price[in];
	int ava=node->avail[in];
	removeKey(k);
	int av;
	cout<<"\nNumber of Items made: ";
	cin>>av;
	insertKey(k,nam,pri,ava+av);
}

void insertKey(int k, char *na, int pr, int av){
	if (root == NULL){
        root = new Btn(t, true);
        root->key[0] = k;
		strcpy(root->name[0], na); 
		root->price[0] = pr; 
		root->avail[0] = av;  
        root->n = 1;  
    }
    
    else{
    	if (root->n == 2*t-1){
        	Btn *s = new Btn(t, false);
        	s->c[0] = root;
        	s->splitChild(0, root);
        	int i = 0;
            if (s->key[0] < k)
                i++;
            s->c[i]->insertNonFull(k,na,pr,av);
            root = s;
        }
        
        else  
            root->insertNonFull(k,na,pr,av);
    }
	}
 
void removeKey(int k){
    if (!root){
        cout << "\nNo Data.";
        return;
    }
    root->removeKey(k);
    if (root->n==0){
        Btn *tmp = root;
        if (root->l)
            root = NULL;
        else
            root = root->c[0];
        delete tmp;
    }
    return;
}
};

/*
*Userinfo class is used for login and logout scenario.
*/
class Userinfo{
	public:
		void login(char *un){
			cout<<"\n\n\aConnected to:";
			cout<<"\nThe VPizza Company: Release 1.0.0.1.1\n";
			cout<<"User: "<<un;
			cout<<"\n\n";
		}
		
		void logout(char *un){
			system("cls");
			cout<<"\nConnection closed to: "<<un;
			cout<<"\nLogged out Sucessfully.\n";
		}
};


/*
*The Map class encapsulates the floydWarshell() and getPath() function used for shortest path.
*/
class Map{
public:
void floydWarshell (int graph[][V], int par[][V])
{
    int dist[V][V], i, j, k;
 
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            dist[i][j] = graph[i][j];
    for (k = 0; k < V; k++)
    {
        for (i = 0; i < V; i++)
        {
            for (j = 0; j < V; j++)
            {
                if (dist[i][k] + dist[k][j] < dist[i][j]){
					par[i][j]=k;
                    dist[i][j] = dist[i][k] + dist[k][j];
				}
            }
        }
    }
}
 
void showPath(int par[][V],int s, int d){
	if(par[s][d]!=-1){
		showPath(par,s,par[s][d]);
		cout<<par[s][d]<<" ";
	}
} 
};


int main(){
/*
*Defination of variables used to input the data.
*/
	int te;
	int tpr, tav, in;
	char ch;
	char tna[20];
	
/*
*Definiton of Objects to use repective functions.
*/
    Bt t(3);
    Btn *node;
	Userinfo u;
	DequeFront  d;
	Map m;
	
	
/*
*Map variables storing the interconnection and their parents.
*/
	int graph[V][V] = { {0,3,8,INF,-4},
                        {INF,0,INF,1,7},
                        {INF,4,0,INF,INF},
                        {2,INF,-5,0,INF},
                        {INF,INF,INF,6,0},
                      };
                      
    int par[V][V] = { {-1,0,0,-1,0},
                        {-1,-1,-1,1,1},
                        {-1,2,-1,-1,-1},
                        {3,-1,3,-1,-1},
                        {-1,-1,-1,4,-1},
                      };	
	
	t.addMenu();
	
	title:
	cout<<"\n\aThe VPizza Company: Release 1.0.0.1.1 Production on Sat Sep 26 22:15:04 2015\n";
	cout<<"\nCoprights (c) 2015, V Inc. All rights reserved.\n";
	
	
	login:
	cout<<"\n\aEnter user-name: ";
	char un[10];
	cin>>un;
	cout<<"\aEnter password: ";
	string pw ="";
	char p;
	p=_getch();
	while(p != 13){
	pw.push_back(p);
    p = _getch();
	}
	
/*
*Validates the login and transfers the control to the user.
*/
	if(!strcmp(un,"root")&&pw=="rootuser"){
		u.login(un);
		goto rootlogin;
	}
	else if(!strcmp(un,"kitchen")&&pw=="kitchenuser"){
		u.login(un);
		goto kitchenlogin;
	}
	else if(!strcmp(un,"counter")&&pw=="counteruser"){
		u.login(un);
		goto counterlogin;
	}
	else if(!strcmp(un,"waiter")&&pw=="waiteruser"){
		u.login(un);
		goto waiterlogin;
	}
	else if(!strcmp(un,"delivery")&&pw=="deliveryuser"){
		u.login(un);
		goto deliverylogin;
	}
	else{
		cout<<"\n\a\aInvalid Username/Password. Logon denied.\n";
		goto login;
	}
	
	rootlogin:	
	do{
		cout<<"\n1.Add Item\n2.Delete Item\n3.Update Item Details";
		cout<<"\n4.Search Item\n5.View Menu";
		cout<<"\t\t\t\tAmount in Hand: Rs."<<vpizzacol;
		cout<<"\n6.Save to File\nEnter the Choice:";
		cin>>ch;
		switch(ch){
			case '1':
				cout<<"\n\aEnter the Item Key: ";
				cin>>te;
				cout<<"\n\aEnter the Item Name: ";
				cin>>tna;
				cout<<"\n\aEnter the Price: ";
				cin>>tpr;
				cout<<"\n\aNumber of Items: ";
				cin>>tav;
				t.insertKey(te,tna,tpr,tav);
				cout<<"\n\aItem inserted sucessfully.";
				break;
				
			case '2':
				cout<<"\a\nEnter the Item Key: ";
				cin>>te;
				t.removeKey(te);
				cout<<"\n\aItem deleted sucessfully.";
				break;
				
			case '3':
				cout<<"\a\nEnter the Item Key:";
				cin>>te;
				t.update(te);
				cout<<"\n\aItem updated Sucessfully.\n";
				break;
				
			case '5':
				cout<<"\n\aID\tName\t\tPrice\tAvailable";
				t.treeTrav();
				break;
				
			case '4':
				cout<<"\a\nEnter the Item Key:";
				cin>>te;
				node=t.searchKey(te,in);
				node->displayContents(in);
				cout<<"\a";
				break;
				
			case '6':
				t.printToFile();
				cout<<"\a\nSaving into File..";
				cout<<"\nFile saved.";
				break;
				
			case '#':
				u.logout(un);
				goto login;
				break;
		}
	}while(1);
	
	
	kitchenlogin:
		do{
		cout<<"\n1.Update Item Details\n2.Search Item\n3.View Menu\nEnter the Choice:";
		cin>>ch;
		switch(ch){
					
			case '1':
				cout<<"\a\nEnter the Item Key:";
				cin>>te;
				t.updateKitchen(te);
				cout<<"\n\aItem updated Sucessfully.\n";
				break;
				
			case '3':
				cout<<"\a\nID\tName\t\tPrice\tAvailable";
				t.treeTrav();
				break;
				
			case '2':
				cout<<"\a\nEnter the Item Key:";
				cin>>te;
				node=t.searchKey(te,in);
				node->displayContents(in);
				break;
				
			case '#':
				u.logout(un);
				goto login;
				break;
		}
	}while(1);


	waiterlogin:
		do{
		cout<<"\n1.View Menu\n2.Orders\n3.Serve\n4.Serve all items\n5.Amount";
		cout<<"\n6.Pay Bill\n7.View Orders\nEnter the Choice:";
		cin>>ch;
		switch(ch){
				
			case '1':
				cout<<"\a\nID\tName\t\tPrice\tAvailable";
				t.treeTrav();
				break;
				
			case '2':
				cout<<"\a\nEnter the Item Key:";
				cin>>te;
				node=t.searchKey(te,in);
				cout<<"\n\aEnter the Quantity:";
				int q;
				cin>>q;
				node->addToListW(in,q);
				t.updateMisc(te,q);
				break;	
			
			case '3':
				cout<<"\a\nServing ";
				cout<<wname[wpf];
				am+=wprice[wpf];
				d.dequeFrontW();
				cout<<endl;
				break;
			
			case '4':
				cout<<"\n\aServing all Items.\n";
				for(int i=wpf;i<=wpr;i++){
					am+=wprice[i];
					cout<<"\nServing "<<wname[i];
					d.dequeFrontW();
				}
				break;
			
			case'5':
				cout<<"\a\nTotal Amount to be Paid: Rs."<<am;
				break;
				
			case '6':
				cout<<"\a\nPaying Bill.";
				vpizzacol+=am;
				am=0;
				break;
				
			case '7':
				cout<<"\a\nItems to be served:";
				for(int i=wpf;i<=wpr;i++){
					cout<<"\n"<<wname[i]<<"\t"<<wq[i];
				}
				break;
				
			case '#':
				am=0;
				u.logout(un);
				goto login;
				break;
		}
	}while(1);


	counterlogin:
		do{
		cout<<"\n1.View Menu\n2.Order\n3.View Orders\n4.Confirm Order";
		cout<<"\t\t\tAmount in Hand: Rs."<<vpizzacol;
		cout<<"\nEnter the Choice:";
		cin>>ch;
		switch(ch){
				
			case '1':
				cout<<"\a\nID\tName\t\tPrice\tAvailable";
				t.treeTrav();
				break;
				
			case '2':
				cout<<"\n\aEnter the Item Key:";
				cin>>te;
				node=t.searchKey(te,in);
				cout<<"\n\aEnter the Quantity:";
				int q;
				cin>>q;
				node->addToListD(in,q);
				t.updateMisc(te,q);
				break;
			
			case '3':
				cout<<"\n\aItems Ordered:";
				for(int i=dpf;i<=dpr;i++){
					cout<<"\n"<<dname[i]<<"\t"<<dq[i];
				}
				break;
				
			case '4':
				cout<<"\a\nOrder Confirmed.";
				break;
				
			case '#':
				u.logout(un);
				goto login;
				break;
		}
	}while(1);
	
	deliverylogin:
		do{
			cout<<"\n1.Orders\n2.Deliver";
			cout<<"\t\t\tAmount in Hand:Rs."<<damt;
			cout<<"\n3.Get Path to Next Location\n4.Pay Bill";
			cout<<"\t\t\tCurrent Location: "<<lo;
			cout<<"\nEnter the Choice:";
			
			cin>>ch;
			switch(ch){
		
			case '1':
				cout<<"\a\nItems to be Delivered:";
				for(int i=dpf;i<=dpr;i++){
					cout<<"\n\a"<<dname[i]<<"\t"<<dq[i]<<"\t"<<dloc[i];
				}
				break;
				
			case '2':
				cout<<"\a\nDelivering Items.\n";
				for(int i=dpf;i<=dpr;i++){
					if(dloc[i]==cdloc){
					damt+=dprice[i];
					cout<<"\nDelivering "<<dname[i];
					d.dequeFrontD();
				}
				}
				cout<<endl;
				break;
				
			case '3':
				cout<<"\n\aEnter the Next Location based on Priority:";
				cin>>lo;
				cout<<"\n\aGo along this path to reach the delivery point Faster.\n";
				m.floydWarshell(graph,par);
    			m.showPath(par,cdloc,lo);
    			cout<<lo;
    			cout<<endl;
    			cdloc=lo;
				break;
				
			case '4':
				cout<<"\a\nPaying Bill.";
				vpizzacol+=damt;
				damt=0;
				break;
				
			case '#':
				u.logout(un);
				goto login;
				break;
		}
	}while(1);

	return 0;
}
