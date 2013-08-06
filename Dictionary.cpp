#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#define LEFT  1
#define RIGHT 2

using namespace std;
struct node
{
 char word[20],meaning[100];
 node *left,*right;
};

node *maketree(char[],char[]);

node* treefromfile();
void filefromtree(node*);
void addword(node*,char[],char[]);
void seperateword(char[],char[],char[]);
void stringToUpper(string);
void findCharacterSeq(node* ,string );
void displayall(node*);
node* bsearch(node*,char[]);
void showmenu();
FILE *file_ptr;

void prog()
{
    //system("CLS");
    char word[20],meaning[100];
    string sequence;
    char menuchoice;
    node *temp;
    temp=treefromfile();
    if(temp==NULL)
     {
      cout<<"File does not exist or dictionary is empty..."<<endl;
     }
    do
    {
        showmenu();
        cout<<"Enter your choice"<<endl;
        cin>>menuchoice;
        switch(menuchoice)
          {
           case '1':cout<<"Enter word : ";
              cin>>word;
              cout<<"Enter meaning : ";
              cin>>meaning;
              cin.getline(meaning,100);
              if(temp==NULL)
              {
                temp=maketree(word,meaning);
              }
              else
               addword(temp,word,meaning);

              break;
           case '2':if(temp==NULL)
               cout<<"The dictionary is empty...";
              else
              {
               cout<<"Find meaning of : ";
               cout.flush();
               cin>>word;
               node *t;
               t=bsearch(temp,word);
              // t=bsearch(temp,meaning);
               if(t==NULL)
                printf("Word not found...");
               else
               {
                printf("%s : ",t->word);
                //printf("%s : ",t->meaning);
                cout<<t->meaning<<endl<<endl;
               }
              }
              break;
           case '3':if(temp==NULL)
               printf("Dictionary is empty...");
              else
               displayall(temp);
              break;
           case '4':
               cout<<"Please enter the sequence to be searched: ";
               cin>>sequence;
               findCharacterSeq(temp,sequence);
               break;
           case '5':filefromtree(temp);
              exit(1);
              break;
           case '6':break;
           default:
              cout << "Not a Valid Choice. \nChoose again.\n";
              break;
          }
          //cout<<"case 1 exit";
    }while(menuchoice!=6);
}
void showmenu()
{
     cout<<endl<<"**************BST DICTIONARY****************"<<endl;
     cout<<"[1]. Add a word."<<endl;
     cout<<"[2].	Find meaning."<<endl;
     cout<<"[3].	Display all."<<endl;
     cout<<"[4].    Display words containing character sequence"<<endl;
     cout<<"[5].    Save and Close"<<endl;
     cout<<"[6].    Exit"<<endl;
}
node* treefromfile()
{
     node *ptree=NULL;
     char word[20],meaning[100],str[120],*i;
     int flags=0;
     file_ptr=fopen("dictionary.txt","r");
     if(file_ptr==NULL)
      ptree=NULL;
     else
     {

      while(!feof(file_ptr))
      {
        i=fgets(str,120,file_ptr);
        if(i==NULL)
        break;
        seperateword(str,word,meaning);
        if(flags==0)
        {
         ptree=maketree(word,meaning);
         flags=1;
        }
        else
         addword(ptree,word,meaning);
      }

        fclose(file_ptr);
     }
     return ptree;
}

node* maketree(char w[],char m[])
    {
     node *p;
     p=new node;
     strcpy(p->word,w);
     strcpy(p->meaning,m);
     p->left=NULL;
     p->right=NULL;
     return p;
}
void seperateword(char str[],char w[],char m[])
{
     int i,j;
     for(i=0;str[i]!=' ';i++)
      w[i]=str[i];
     w[i++]=NULL;	//Append the null and skip the space.
     for(j=0;str[i]!=' ';i++,j++)
     {
        m[j]=str[i];
     }
     m[j]=NULL;
}

void addword(node *tree,char word[],char meaning[])
{
     node *p,*q;
     p=q=(tree);
     if(p->left==NULL)
 while(p!=NULL && strcmp(word,p->word)!=0)
     {
        q=p;
      if(strcmp(word,p->word)<0)
      {
                p=p->left;
      }
      else
      {
                p=p->right;
      }
     }
     if(strcmp(word,q->word)==0)
     {
      cout<<"This word already exists..."<<endl;
      //delay(1000);
     }
     else if(strcmp(word,q->word)<0)
      q->left=maketree(word,meaning);
     else
      q->right=maketree(word,meaning);

    }

node* bsearch(node *tree,char word[])
    {
     bool wordFound = false;
     node *q;
     q=tree;
     while(q!=NULL)
     {
      //p=q;
      if(strcmp(word,q->word)<0)
       {
           //cout<<"Traversing Left";
           q=q->left;
       }
      else if(strcmp(word,q->word)>0)
       {
           //cout<<"Traversing Right";
           q=q->right;
       }
       //cout<<"Traversed :"<<q->word;
      if(q!=NULL && strcmp(word,q->word)==0)
      {
          wordFound = true;
          break;
      }

     }
     return q;
}


void filefromtree(node *tree)
    {
     void travandwrite(node*);
     file_ptr=fopen("dictionary.txt","w");
     if(file_ptr==NULL)
     {
      printf("Cannot open file for writing data...");
     }
     else //if(tree==NULL)
     {
      if(tree!=NULL)
      {
       travandwrite(tree);
      }
      fclose(file_ptr);  //Close the file anyway.
     }
}
void travandwrite(node *tree)
{
 if(tree!=NULL)
 {
  fprintf(file_ptr,"%s %s \n",tree->word,tree->meaning);
  travandwrite(tree->left);
  travandwrite(tree->right);
 }
}
void displayall(node *tree)
{
 if(tree!=NULL)
 {
  displayall(tree->left);
  cout<<tree->word<<":"<<tree->meaning<<endl;
  displayall(tree->right);
 }
}

/*
Finds the sequence of characters in Disctionary
PRE: Root (tree) and seqeunce of characters is passed.
POST: Prints the words which contains that sequence.
*/
void findCharacterSeq(node* tree,string sequence)
{
   if(tree!=NULL)
     {
      findCharacterSeq(tree->left,sequence);
      string word = tree->word;
      transform(word.begin(), word.end(), word.begin(), (int(*)(int)) toupper);
      transform(sequence.begin(), sequence.end(), sequence.begin(), (int(*)(int)) toupper);
      std::size_t found = word.find(sequence);
      if(found!=std::string::npos)
        cout<<tree->word<<":"<<tree->meaning<<endl;
      findCharacterSeq(tree->right,sequence);
     }
}

/*
Converts String to UpperCase
Pre: String to be converted to upper case
PosT: string is converted to uppercase
*/
void stringToUpper(string &str)
{
   for(unsigned int l = 0; l < str.length(); l++)
  {
    str[l] = toupper(str[l]);
  }
}
void intro()
{
    int i;
    system("CLS");
    //gotoxy(20,20);
    cout<<"DICTIONARY LOADING";
    for(i=0;i<50;i++)
    {
    // gotoxy(15+i,21);
     cout<<"þþþ";
    // gotoxy(20,22);
     cout<<2*i<<"% completed";
    // delay(150);
}
    //gotoxy(20,20);
    cout<<"DICTIONARY LOADING COMPLETED";
    system("CLS");
}


int main()
{
    system("CLS");
   // intro();
    prog();
}
