#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */



int Fsize=50 * sizeof(char); /*maximum formula length*/
int inputs =10;/* number of formulas expected in input.txt*/
int i;/* in case you need it */
int ThSize=100;/* maximum size of set of formulas*/
int TabSize=500; /*maximum length of tableau queue*/



/* A set will contain a list of words. Use NULL for emptyset.  */
  struct set{
    char *item;/*first word of non-empty set*/
    struct set *tail;/*remaining words in the set*/
  };

/* A tableau will contain a list of pointers to sets (of words).  Use NULL for empty list.*/
struct tableau {
  struct set * S; /* pointer to first set in non-empty list */
  struct tableau *rest; /*list of pointers to other sets*/
};


/*You need to change this next bit and include functions for parse, closed and complete.*/
int isExpand(struct set *s);
int isClosed(struct set *s);
int isLiteral(char *formula);
int parse(char* name);
int findBinary(char* name);
int isNeg(char* name);
int isBinary(char* name);
int isProp(char* name);
char *parttwo(char *name);
char *partone(char *name);
struct tableau* findTail(struct tableau *t);
int isAlpha(char* formula);
struct tableau *complete(struct tableau *t);
struct set *cloneSet(struct set* thisSet);
void printSet(struct set *s);
void printTab(struct tableau *t);
char *rulepartone(char *name);
char *ruleparttwo(char *name);
int closed(struct tableau *t);
int isAllExpand(struct tableau *t);












/*You should not need to alter the program below.*/
int main()
{ 

  // char a[50] =  "-(p>(q>p))";
  // struct set S={a, NULL};
  // struct tableau t={&S, NULL};



/*  struct set *test = malloc(sizeof(struct set));
  test->item = malloc(sizeof(Fsize));
  test->tail = malloc(sizeof(struct set));
  test->tail->item = malloc(sizeof(Fsize));
  test->tail->tail = malloc(sizeof(struct set));
  test->tail->tail->item = malloc(sizeof(Fsize));
  strcpy(test->item, "q");
  strcpy(test->tail->item, "q");
  strcpy(test->tail->tail->item, "-q"); */
  // struct tableau *newT = complete(&t);
  // printTab(newT);
  // printf("%d\n", isClosed(newT->S));
  //  printf("%d\n", isClosed(newT->rest->S));

 // printf("%s\n", rulepartone(a));
 // printf("%s\n", ruleparttwo(a));


   char *name = malloc(Fsize);
  char *name2 = malloc(Fsize);
  FILE *fp, *fpout;
 

  if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<inputs;j++)
    {
      fscanf(fp, "%s",name);
      strcpy(name2, name);
      switch (parse(name))
      {
          case(0): fprintf(fpout, "%s is not a formula.  \n", name);break;
          case(1): fprintf(fpout, "%s is a proposition. \n ", name);break;
          case(2): fprintf(fpout, "%s is a negation.  \n", name);break;
          case(3):fprintf(fpout, "%s is a binary. The first part is %s and the second part is %s  \n", name, partone(name), parttwo(name));break;
          default:fprintf(fpout, "What the f***!  ");
              
      }

    
      struct set S={name, NULL};
      struct tableau t={&S, NULL};
      if (parse(name)!=0)
	    {
        struct tableau *newT = complete(&t);
        printTab(newT);
	      if (closed(newT))  fprintf(fpout, "%s is not satisfiable.\n", name2);
	      else fprintf(fpout, "%s is satisfiable.\n", name2);
	    }
      else  fprintf(fpout, "I told you, %s is not a formula.\n", name2);
   }

 
  fclose(fp);
  fclose(fpout);
  free(name);
  return(0);
}




struct tableau *complete(struct tableau *t)
{
  char *ssstr = malloc(Fsize);
  strcpy(ssstr, t->S->item);  
  while(t != NULL)
  {
    struct tableau *currentSet = t;
    struct set *currentWords = currentSet->S;
   // currentSet = currentSet->rest;
  //  if (isExpand(currentSet->S) && isClosed(currentSet->S))
  //  {
  //       struct tableau *beta1 = malloc(sizeof(struct tableau));
  //       beta1->S = cloneSet(currentSet->S);
  //       beta1->rest = NULL;
  //         if (t == NULL)
  //         {
  //           t = beta1;
  //         }
  //         else
  //         {
  //           findTail(t)->rest = beta1;
  //         }
  //  }
    t = t->rest;
    if (isAllExpand(currentSet))
    {
      // printf("satisfiable");
      return currentSet;
    }
    else
    {
      while(isLiteral(currentWords->item)) /*******************************************/
      {
        currentWords = currentWords->tail;
        if(currentWords == NULL)
        {
          struct tableau *beta2 = malloc(sizeof(struct tableau));
          beta2->S = cloneSet(currentSet->S);
          beta2->rest = NULL;
          if (t == NULL)
          {
            t = beta2;
          }
          else
          {
            findTail(t)->rest = beta2;
          }
        }
      }
      if(isAlpha(currentWords->item))
      {
        char *formula = malloc(Fsize);
        strcpy(formula, currentWords->item);
         /*************************/
        char *formula1 = malloc(Fsize);
        strcpy(formula1, rulepartone(formula));
        char *formula2 = malloc(Fsize);
        strcpy(formula2, ruleparttwo(formula));
        strcpy(currentWords->item, formula1);
        printf("%s-*****\n", formula);
        struct set *alpha2 = malloc(sizeof(struct set));
        alpha2->item = malloc(Fsize);
        strcpy(alpha2->item, formula2);
        alpha2->tail = currentWords->tail;
        currentWords->tail = alpha2;
        currentSet->rest = NULL;
          if (t == NULL)
          {
            t = currentSet;
          }
          else
          {
            findTail(t)->rest = currentSet;
          }
        // }
      }
      else
      {
        char *formula = malloc(Fsize);
        strcpy(formula, currentWords->item);
         /*************************/
        char *formula1 = malloc(Fsize);
        strcpy(formula1, rulepartone(formula));
        char *formula2 = malloc(Fsize);
        strcpy(formula2, ruleparttwo(formula));

        strcpy(currentWords->item,formula1);//************* 
        struct tableau *beta1 = malloc(sizeof(struct tableau));
        beta1->S = cloneSet(currentSet->S);
        beta1->rest = NULL;
          if (t == NULL)
          {
            t = beta1;
          }
          else
          {
            findTail(t)->rest = beta1;
          }
        // }
        strcpy(currentWords->item, formula2);
        struct tableau *beta2 = malloc(sizeof(struct tableau));
        beta2->S = cloneSet(currentSet->S);
        // beta2->S = currentSet->S;
        beta2->rest = NULL;
        // if(!isClosed(beta2->S))
        // {
          if (t == NULL)
          {
            t = beta2;
          }
          else
          {
            findTail(t)->rest = beta2;
          }
          //printf("_______---------______%d\ns", findTail(t) == beta1.tail);
        // }
      }
    }
  }
  return NULL;
}


struct tableau* findTail(struct tableau *t)
{
  while(t->rest != NULL)
  {
    t = t->rest;
  }
  return t;
}

int isClosed(struct set *s)
{
  while(s->tail != NULL)
  {
  if(isLiteral(s->item))
  {
  struct set *temp = s->tail;
    while(temp != NULL)
    {
      if(isLiteral(temp->item))
      {
        if(*(temp->item + 1) == *(s->item) || *(temp->item) == *(s->item + 1))
        {
          return 1;
        }
      }
      temp = temp->tail;
    }
  }
  s = s->tail;
  }
  return 0;
}

int closed2(struct tableau *t)
{
  while(t != NULL)
  {
    if (isClosed(t->S))
    {
      return 1;
    }
    t = t->rest;
  }
  return 0;
}

int closed(struct tableau *t)
{
  while(t != NULL)
  {
    if (!isClosed(t->S))
    {
      return 0;
    }
    t = t->rest;
  }
  return 1;
}


/*int mockLiteral(char *str);
int isContra(char* str1,char* str2);
int isClosed(struct set *s)
{
  if (s== NULL){
    return 0;
  }
  do{
  if(mockLiteral(s->item) != 4)
  {
    struct set *temp = s;
    do
    {
      if(mockLiteral(temp->item) != 4)
      {
        if(isContra(temp->item,s->item))
        {
          return 1;
        }
      }
      temp = temp->tail;
    }while(temp != NULL);
  }
  s = s->tail;
  }while(s != NULL);
  return 0;
}

int mockLiteral(char *str){
  if(strlen(str)>2 || strlen(str)==0)
  return 4;
  if (strlen(str)==1){
    switch(*str){
      case('p'):
        return 1;
      case('q'):
        return 2;
      case('r'):
        return 3;
    }
  }
  if (strlen(str)==1 && *str == '-'){
    switch(*(str+1)){
      case('p'):
        return -1;
      case('q'):
        return -2;
      case('r'):
        return -3;
    }
  }
  return 4;
}

int isContra(char* str1,char* str2){
  if (mockLiteral(str1) + mockLiteral(str2) == 0){
    return 1;
  }
  return 0;
}*/



int isLiteral(char *formula)
{
  if(isProp(formula)) {return 1; }
  else if (isBinary(formula)){return 0;}
  else if (isNeg(formula))
  {
    char insider[50] = "";
    strncpy(insider, formula + 1, strlen(formula) - 1);
    return isLiteral(insider);
  }
  else{return 0;}
  
}

int isNeg(char* name)
{
    if (*name == '-')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int isBinary(char* name)
{
    if(*name == '(' && *(name + strlen(name) - 1) == ')')
    {
        return findBinary(name);
    }
    return 0;
}

int isProp(char* name)
{
    if ((strcmp(name, "p") == 0) || (strcmp(name, "q") == 0) || (strcmp(name, "r") == 0))
    {
        return 1;
    } 
    else
    {
        return 0;
    }
}

int findBinary(char* name)
{
    int i;
    int counter = 0;
    for(i = 1; i < strlen(name) - 1; i++)
    {
        if (*(name + i) == '(')
        {
            counter++;
           // printf("%d %d\n", i, counter);
        }
        else if (*(name + i) == ')')
        {
            counter--;
           // printf("%d %d\n", i, counter);
        }
        else if (((*(name + i) == 'v') || (*(name + i) == '^') || (*(name + i) == '>')) && (counter == 0))
        {
          //  printf("%d\n", i);
            return(i);
        }
    }
    return 0;
}

int parse(char* name)
{
    // printf("%s\n", name);
    if (isProp(name))
    {
        return(1);
    }
    if (isNeg(name))
    {
        char insider[50] = "";
        strncpy(insider, name + 1, strlen(name) - 1);
        if(!parse(insider))
        {
            return(0);
        }
        return(2);
    }
    if (isBinary(name))
    {
       // printf("been here\n");
        int bcpos = isBinary(name);
        char partonestr[50];

        strcpy(partonestr, partone(name));
        char parttwostr[50];
        strcpy(parttwostr, parttwo(name));
        // printf("----------------------");
        // printf("%s\n", partonestr);
        // printf("%s\n", parttwostr);
        if(!parse(partonestr))
        {
            return(0);
        }
        if(!parse(parttwostr))
        {
            return(0);
        }
        return(3);
    }
    return(0);
}



char *partone(char *name)
{
    int bcpos = findBinary(name);
    char *result = malloc(Fsize);
    strncpy(result, name + 1, bcpos - 1);
    // printf("%s\n", result);
  //  ascii(result);
    return result;
}

char *parttwo(char *name)
{
    int bcpos = findBinary(name);
    char *result = malloc(Fsize);
    strncpy(result, name + bcpos + 1, strlen(name) - bcpos - 2);
    // printf("%s\n", result);
  //  ascii(result);
    return result;
}


struct set *cloneSet(struct set* thisSet)
{
  struct set *copied = malloc(sizeof(struct set));
  copied->item = malloc(Fsize);
  strcpy(copied->item,thisSet->item);
  // copied->item = thisSet->item;
  if (thisSet->tail != NULL)
  {
  copied->tail = cloneSet(thisSet->tail);
  }
  else
  {
    copied->tail = NULL;
  }
  
  return copied;
}

int isExpand(struct set *s)
{
  if (s->tail == NULL)
  {
    return(isLiteral(s->item));
  }  
  while(s != NULL)
  {
    if(!isLiteral(s->item))
    {
      return 0;
    }
    else
    {
      s = s->tail;
    }    
  }
  return 1;
}

int isAllExpand(struct tableau *t)
{
  while(t != NULL)
  {
    if (!isExpand(t->S))
    {
      return 0;
    }
    t = t->rest;
  }
  return 1;
}

void printTab(struct tableau *t)
{
  while(t != NULL)
  {
    printSet(t->S);
    printf("----------------\n");
    t = t->rest;
  }
  return;
}

void printSet(struct set *s)
{
  while(s != NULL)
  {
    printf("++%s++|", s->item);
    s = s->tail;
  }
  return;
}

char *rulepartone(char *name)
{
    if(!isNeg(name))
    {
      if(*(name + findBinary(name)) == '>')
      {
        char *result = malloc(Fsize);
        strcpy(result, "-");
        strcat(result, partone(name));
        return (result);
      }
      else
      {
        return partone(name);
      }      
    }
    else if(*(name + 1 + findBinary(name + 1)) == 'v')
    {
      char *result = malloc(Fsize);
      strcpy(result, "-");
      strcat(result, partone(name + 1));
      return (result);
    }
    else if(*(name + 1 + findBinary(name + 1)) == '>')
    {
      return partone(name + 1);
    }
    else if(*(name + 1 + findBinary(name + 1)) == '^')
    {
      char *result = malloc(Fsize);
      strcpy(result, "-");
      strcat(result, partone(name + 1));
      return (result);
    }
    return NULL;
}

char *ruleparttwo(char *name)
{
    if(!isNeg(name))
    {
      return parttwo(name);
    }
    else if(*(name + 1 + findBinary(name + 1)) == 'v')
    {
      char *result = malloc(Fsize);
      strcpy(result, "-");
      strcat(result, parttwo(name + 1));
      return (result);
    }
    else if(*(name + 1 + findBinary(name + 1)) == '>')
    {
      char *result = malloc(Fsize);
      strcpy(result, "-");
      strcat(result, parttwo(name + 1));
      return (result);
    }
    else if(*(name + 1 + findBinary(name + 1)) == '^')
    {
      char *result = malloc(Fsize);
      strcpy(result, "-");
      strcat(result, parttwo(name + 1));
      return (result);
    }
    return NULL;
}

char *parttwo2(char *name)
{
    int bcpos = findBinary(name);
    char *result = malloc(Fsize);
    strncpy(result, name + bcpos + 1, strlen(name) - bcpos - 2);
    // printf("%s\n", result);
  //  ascii(result);
    return result;
}


int isAlpha(char* formula)
{
  if(isBinary(formula))
  {
    if(*(formula + findBinary(formula)) == 'v' || *(formula + findBinary(formula)) == '>') {return 0;}
    else return 1;
  }
  else if(isNeg(formula))
  {
    return (!isAlpha(formula + 1));
  }
  else
  {
    return -1;
  }
  return 0;
}