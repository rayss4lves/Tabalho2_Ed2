typedef struct InglesBin
{
  char palavraIngles[50];
  struct ArvoreBinaria *esq;
  struct ArvoreBinaria *dir;
} InglesBin;

typedef struct Info
{
  InglesBin *palavraIngles;
  char *palavraPortugues;
} Info;

typedef struct PortuguesRB
{
  Info info;
  int cor;
  struct PortuguesRB *esq;
  struct PortuguesRB *dir;
} PortuguesRB;