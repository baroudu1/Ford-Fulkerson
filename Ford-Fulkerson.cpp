#include <stdlib.h>
#include <stdio.h>
#define infinie 1316134911

typedef struct Noeud
{
	int info;
	struct Noeud *suc;
} noeud;

typedef noeud *listeNoeuds;

typedef struct Arc
{
	noeud *S;
	noeud *D;
	float val;
	float cap;
	float born;
	int etat;
	struct Arc *suc;
} arc;

typedef arc *listeArcs;

typedef struct Graphe
{
	listeNoeuds LN;
	listeArcs LA;
} graphe;

noeud *creerNoeud(int v)
{
	noeud *n = NULL;

	n = (noeud *)malloc(sizeof(noeud));

	n->info = v;
	n->suc = NULL;

	return n;
}

arc *creerArc(noeud *NS, noeud *ND, float b, float v, int e, float c)
{
	arc *arc1 = NULL;

	arc1 = (arc *)malloc(sizeof(arc));

	arc1->D = ND;
	arc1->S = NS;
	arc1->val = v;
	arc1->cap = c;
	arc1->born = b;
	arc1->etat = e;
	arc1->suc = NULL;

	return arc1;
}

graphe creerGraphe()
{
	graphe graphe1;

	graphe1.LA = NULL;
	graphe1.LN = NULL;

	return graphe1;
}

int grapheEstVide(graphe G)
{
	if (!G.LN)
		return 1;

	return 0;
}

int aucunArc(graphe G)
{
	if (!G.LA)
		return 1;

	return 0;
}

void afficherNoeud(noeud *N)
{
	printf("%d", N->info);
}
int afficherNoeudsGraphe(graphe G)
{
	noeud *noeudTest = G.LN;

	if (!grapheEstVide(G))
	{
		while (noeudTest)
		{
			afficherNoeud(noeudTest);
			printf("\t");
			noeudTest = noeudTest->suc;
		}
		printf("\n");
		return 1;
	}
	return 0;
}

int afficherArcsGraphe(graphe G)
{
	arc *arcTest = G.LA;

	if (aucunArc(G) == 0)
	{
		// printf("les arcs du graphe : \n");

		while (arcTest)
		{
			printf("( ");
			afficherNoeud(arcTest->S);
			printf(" , ");
			afficherNoeud(arcTest->D);
			printf(" , %.0f[%.0f,%.0f]) \n", arcTest->val,arcTest->born, arcTest->cap);
			arcTest = arcTest->suc;
		}
		return 1;
	}
	return 0;
}

arc *reverse(arc *head)
{
	if (head == NULL || head->suc == NULL)
	{
		return head;
	}

	arc *list_to_do = head->suc;

	arc *reversed_list = head;
	reversed_list->suc = NULL;

	while (list_to_do != NULL)
	{
		arc *temp = list_to_do;
		list_to_do = list_to_do->suc;

		temp->suc = reversed_list;
		reversed_list = temp;
	}

	return reversed_list;
}

int afficherLISTE(graphe G)
{
	arc *arcTest;
	arcTest = reverse(G.LA);

	if (aucunArc(G) == 0)
	{

		while (arcTest)
		{
			if (arcTest->etat == 1)
			{
				printf(" -----> ");
				afficherNoeud(arcTest->D);
			}

			else
			{
				printf(" <----- ");
				afficherNoeud(arcTest->S);
			}

			arcTest = arcTest->suc;
		}
		return 1;
	}
	return 0;
}

noeud *insererNoeud(int Ninfo, listeNoeuds *LN)
{
	noeud *noeudNew;
	noeudNew = *LN;
	if (!noeudNew)
	{
		*LN = creerNoeud(Ninfo);
		return *LN;
	}
	while (noeudNew)
	{
		if (noeudNew->info == Ninfo)
			return noeudNew;
		if (!noeudNew->suc)
		{
			noeudNew->suc = creerNoeud(Ninfo);
			return noeudNew->suc;
		}
		noeudNew = noeudNew->suc;
	}
	return noeudNew;
}

arc *insererArc(noeud *NS, noeud *ND, float b, float v, float c, int e, listeArcs *LA)
{
	arc *arcNew = *LA;
	if (!arcNew)
	{
		*LA = creerArc(NS, ND, b, v, e, c);
		return *LA;
	}
	while (arcNew)
	{
		if (arcNew->S->info == NS->info && arcNew->D->info == ND->info)
			return arcNew;
		if (!arcNew->suc)
		{
			arcNew->suc = creerArc(NS, ND, b, v, e, c);
			return arcNew->suc;
		}
		arcNew = arcNew->suc;
	}
	return arcNew;
}

noeud *insererNoeudGraphe(int v, graphe *G)
{
	return insererNoeud(v, &(G->LN));
}

arc *insererArcGraphe(int NS, int ND, float b, float v, float c, graphe *G)
{
	return insererArc(insererNoeud(NS, &G->LN), insererNoeud(ND, &G->LN), b, v, c, 0, &G->LA);
}

int existNoeud(int v, graphe G)
{
	noeud *noeudNew = G.LN;

	if (grapheEstVide(G))
		return 0;
	while (noeudNew)
	{
		if (noeudNew->info == v)
			return 1;
		noeudNew = noeudNew->suc;
	}
	return 0;
}

int existArc(int Sinfo, int Dinfo, graphe G)
{
	arc *arcNew = G.LA;

	if (aucunArc(G))
		return 0;
	while (arcNew)
	{
		if (arcNew->S->info == Sinfo && arcNew->D->info == Dinfo)
			return 1;
		arcNew = arcNew->suc;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
graphe GLOBALL = creerGraphe();

int listeAug(int infoS, int infoP, graphe G, graphe *GG)
{
	graphe A = G;
	if (existNoeud(infoS, G) && existNoeud(infoP, G))
	{
		if (existArc(infoS, infoP, G))
		{
			while (A.LA)
			{

				if (A.LA->S->info == infoS && A.LA->D->info == infoP)
				{
					if (A.LA->val < A.LA->cap)
					{
						insererArc(A.LA->S, A.LA->D, A.LA->born, A.LA->val, A.LA->cap, 1, &GG->LA);
						return 1;
					}
				}
				A.LA = A.LA->suc;
			}
		}
		graphe S = G;
		while (S.LA)
		{
			if (!existArc(S.LA->S->info, S.LA->D->info, GLOBALL))
			{
				if (S.LA->S->info == infoS)
				{
					insererArc(S.LA->S, S.LA->D, S.LA->born, S.LA->val, S.LA->cap, 1, &GLOBALL.LA);
					if (S.LA->val < S.LA->cap)
					{
						if (listeAug(S.LA->D->info, infoP, G, GG))
						{
							insererArc(S.LA->S, S.LA->D, S.LA->born, S.LA->val, S.LA->cap, 1, &GG->LA);
							return 1;
						}
					}
				}
				if (S.LA->D->info == infoS)
				{
					insererArc(S.LA->S, S.LA->D, S.LA->born, S.LA->val, S.LA->cap, 1, &GLOBALL.LA);
					if (S.LA->val > S.LA->born)
					{
						if (listeAug(S.LA->S->info, infoP, G, GG))
						{
							insererArc(S.LA->S, S.LA->D, S.LA->born, S.LA->val, S.LA->cap, -1, &GG->LA);
							return 1;
						}
					}
				}
			}
			S.LA = S.LA->suc;
		}
	}
	return 0;
}
int fordFELL(int infoS, int infoP, graphe G, FILE **fr)
{
	int cout = 0;
	graphe GG;
	GG = creerGraphe();
	int i;
	while (listeAug(infoS, infoP, G, &GG))
	{
		GLOBALL = creerGraphe();
		int k = infinie;

		graphe AA = GG;

		while (AA.LA)
		{
			if (AA.LA->etat == 1)
			{
				if (k > AA.LA->cap - AA.LA->val)
					k = AA.LA->cap - AA.LA->val;
			}
			if (AA.LA->etat == -1)
			{
				if (k > AA.LA->val)
					k = AA.LA->val;
			}
			AA.LA = AA.LA->suc;
		}
		graphe GGG = GG;

		cout += k;
		while (GG.LA)
		{
			graphe A;
			A = G;
			while (A.LA)
			{
				if (A.LA->S->info == GG.LA->S->info && A.LA->D->info == GG.LA->D->info)
				{
					if (GG.LA->etat == 1)
						A.LA->val += k;
					else
						A.LA->val -= k;
				}
				A.LA = A.LA->suc;
			}

			GG.LA = GG.LA->suc;
		}
		printf("Liste augmentente : \n");
		printf("%d ", infoS);
		afficherLISTE(GGG);
		printf("\nK= %d\n", k);
	}

	printf("La valeur de flot Max = %d\nLes flux :\n", cout);
	fprintf(*fr, "La valeur de flot Max = %d\nLes flux :\n", cout);
	afficherArcsGraphe(G);
	while (G.LA)
	{
		fprintf(*fr, "%d\t%d\t%.0f\n", G.LA->S->info, G.LA->D->info, G.LA->val);
		G.LA = G.LA->suc;
	}
	return cout;
}

int lireFichier(graphe *G, FILE **fp, char *nomFichier, int *p)
{
	*fp = fopen(nomFichier, "r");
	int infoS, infoD;
	float born,flus,cap;

	if (*fp != NULL)
	{
		int j, i = -2;
		while (!feof(*fp))
		{
			if (i == -2)
				fscanf(*fp, "%d\n", &j);
			else if (i == -1)
				fscanf(*fp, "%d\n", p);
			else
			{
				fscanf(*fp, "%d\t%d\t%f\t%f\t%f\n", &infoS, &infoD,&born,&flus, &cap);
				insererArcGraphe(infoS, infoD, born, flus, cap, G);
			}
			i++;
		}
		return j;
	}
	return NULL;
}

int main()
{
	int NS, NP;
	graphe G;
	G = creerGraphe();

	FILE *fp;
	FILE *fr;
	char file[] = "file.txt";
	NS = lireFichier(&G, &fp, file, &NP);
	printf("les arcs du graphe : \n");
	afficherArcsGraphe(G);
	printf("\n");
	graphe GG;
	GG = creerGraphe();

	fr = fopen("outputFORD.txt", "w+");
	fordFELL(NS, NP, G, &fr);
	fclose(fr);
	system("pause");
}
