#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#define LEFT  1
#define RIGHT 2

using namespace std;
struct node
{
 char word[20],meaning[100];
 node *left,*right;
};

node *makeTree(char[],char[]);
node* treeFromFile();
void fileFromTree(node*);
void addWord(node*,char[],char[]);
void seperateWord(char[],char[],char[]);
void stringToUpper(string);
void searchSequence(node* ,string );
void displayAll(node*);
void editMeaning(node* ,char[] );
node* bSearch(node*,char[]);
void showMenu();
FILE *dictionaryFile;


/*
Presents the user with the main menu and calls the relevant functions correspoding to the users inputs
PRE: Nothing
POST: Presents main menu to the users and calls the relevant functions
*/
void mainMenu()
{
    char word[20],meaning[100];
    string sequence;
    char menuChoice;
    node *tempTree;
    tempTree=treeFromFile();
    if(tempTree==NULL)
     {
      cout<<"File does not exist or dictionary is empty..."<<endl;
     }
    do
    {
        showMenu();
        cout<<"Enter your choice"<<endl;
        cin>>menuChoice;
        switch(menuChoice)
          {
           case '1':cout<<"Enter word : ";
              cin>>word;
              cin.ignore();
              cout<<"Enter meaning : ";
              cin.getline(meaning,99);
              if(tempTree==NULL)
              {
                tempTree=makeTree(word,meaning);
              }
              else
               addWord(tempTree,word,meaning);
               break;
           case '2':if(tempTree==NULL)
               cout<<"The dictionary is empty...";
              else
              {
               cout<<"Find meaning of : ";
               cout.flush();
               cin>>word;
               node *t;
               t=bSearch(tempTree,word);
               if(t==NULL)
                printf("Word not found...");
               else
               {
                printf("%s : ",t->word);
                cout<<t->meaning<<endl<<endl;
               }
              }
              break;
           case '3':if(tempTree==NULL)
               printf("Dictionary is empty...");
              else
               displayAll(tempTree);
              break;
           case '4':
               cout<<"Please enter the sequence to be searched: ";
               cin>>sequence;
               searchSequence(tempTree,sequence);
               break;
           case '5':
              cout<<"Enter word to modify"<<endl;
              cin>>word;
              editMeaning(tempTree,word);
              break;
           case '6':fileFromTree(tempTree);
              exit(1);
              break;
           case '7':exit(1);
           default:
              cout << "Not a Valid Choice. \nChoose again.\n";
              break;
          }
    }while(menuChoice!=7);
}


/*
Displays Main Menu
PRE: Nothing
POST: Main Menu is displayed
*/
void showMenu()
{
     cout<<endl<<"**************BST DICTIONARY****************"<<endl;
     cout<<"[1].    Add a word."<<endl;
     cout<<"[2].    Find meaning."<<endl;
     cout<<"[3].    Display all."<<endl;
     cout<<"[4].    Display words containing character sequence"<<endl;
     cout<<"[5].    Edit Meaning"<<endl;
     cout<<"[6].    Save and Close"<<endl;
     cout<<"[7].    Exit"<<endl;
}

/*
Retrieves data from the file dictionary.txt and populates the tree
PRE: Nothing
POST: The tree is created and populated from the data from the file.
*/
node* treeFromFile()
{
     node *ptree=NULL;
     char word[20],meaning[100];
     string strWord, strmeaning;
     int flags=0;
    ifstream file("dictionary.txt");
     if(file==NULL)
      ptree=NULL;
     else
      {
       string str;
       while(getline(file,str))
       {
         int pos = str.find(':');
         strWord = string(str.begin(),str.begin()+pos);
         strmeaning =  string(str.begin()+pos+1,str.end());
         strcpy(word,strWord.c_str());
        strcpy(meaning,strmeaning.c_str());

         if(flags==0)
          {
            ptree=makeTree(word,meaning);
           flags=1;
         }
         else
          addWord(ptree,word,meaning);
       }
      }

     return ptree;
}

/*
Creates tree
PRE: The word and the corresponding meaning.
POST: New node with is created with word and meaning as the data.
*/
node* makeTree(char w[],char m[])
    {
     node *p;
     p=new node;
     strcpy(p->word,w);
     strcpy(p->meaning,m);
     p->left=NULL;
     p->right=NULL;
     return p;
}
/*
Adds a word and meaning to the existing tree
PRE: The tree, word and the corresponding meaning.
POST: The word and meaning is added to the tree if the word is not already existing.
*/
void addWord(node *tree,char word[],char meaning[])
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
     }
     else if(strcmp(word,q->word)<0)
      q->left=makeTree(word,meaning);
     else
      q->right=makeTree(word,meaning);

    }
/*
Searches the tree for a particular word
PRE: Tree and the word
POST: Returns the node if the word is found or null if it doesnt.
*/
node* bSearch(node *tree,char word[])
    {
     bool wordFound = false;
     node *q;
     q=tree;
     while(q!=NULL)
     {
      if(strcmp(word,q->word)<0)
       {
           q=q->left;
       }
      else if(strcmp(word,q->word)>0)
       {
           q=q->right;
       }
      if(q!=NULL && strcmp(word,q->word)==0)
      {
          wordFound = true;
          break;
      }

     }
     return q;
}

/*
Creates file with the data from the tree
PRE: The tree
POST: New file is created with the data from the tree if the file doesnt exist or the tree data is added to the file if the file is already created.
*/
void fileFromTree(node *tree)
    {
     void travAndWrite(node*);
     dictionaryFile=fopen("dictionary.txt","w");
     if(dictionaryFile==NULL)
     {
      printf("Cannot open file for writing data...");
     }
     else
     {
      if(tree!=NULL)
      {
       travAndWrite(tree);
      }
      fclose(dictionaryFile);
     }
}
/*
Traverses through the tree
PRE: The tree
POST: The tree is traversed
*/
void travAndWrite(node *tree)
{
 if(tree!=NULL)
 {
  fprintf(dictionaryFile,"%s:%s\n",tree->word,tree->meaning);
  travAndWrite(tree->left);
  travAndWrite(tree->right);
 }
}
/*
Displays all the data from the tree
PRE: The tree
POST: Displays the data from the tree
*/
void displayAll(node *tree)
{
 if(tree!=NULL)
 {
  displayAll(tree->left);
  cout<<tree->word<<":"<<tree->meaning<<endl;
  displayAll(tree->right);
 }
}

/*
Finds the sequence of characters in Disctionary
PRE: Root (tree) and seqeunce of characters is passed.
POST: Prints the words which contains that sequence.
*/
void searchSequence(node* tree,string sequence)
{
   if(tree!=NULL)
     {
      searchSequence(tree->left,sequence);
      string word = tree->word;
      transform(word.begin(), word.end(), word.begin(), (int(*)(int)) toupper);
      transform(sequence.begin(), sequence.end(), sequence.begin(), (int(*)(int)) toupper);
      std::size_t found = word.find(sequence);
      if(found!=std::string::npos)
        cout<<tree->word<<":"<<tree->meaning<<endl;
      searchSequence(tree->right,sequence);
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

/*
    Allows user to modify meaning of a word in dictionary
    PRE: Tree root and word whose meaning to be modified.
    POST: word meaning is modified if word exists in BST.
          else word not found message is displayed.
*/
void editMeaning(node* tree,char word[])
{
    string newMeaning;
    if(tree==NULL)
               cout<<"The dictionary is empty...";
    else
    {

               node *t;
               t=bSearch(tree,word);
               if(t==NULL)
                cout<<"Word not found..."<<endl;
               else
               {
                    cout<<"Modify meaning to:";
                    cin.ignore();
                    getline(cin,newMeaning);
                    strcpy(t->meaning,newMeaning.c_str());
               }
      }
}
int main()
{
    mainMenu();
}
