/* FUNCTION DEFINITIONS */

#include <iostream>
#include <thread>
#include <chrono>
//#include <cmath>
#include <ctime>
#include "QuadEdge.h"

using namespace std;

//DEFAULT CONSTRUCTOR
QuadEdge::QuadEdge()
{
	//Initialize
	Vertices.clear();
	Edges.clear();
	Faces.clear();
	ConvexHull.clear();
	C.Points.clear();
	Voronoi.clear();
}

//OVERLOAD CONSTRUCTOR (from PointSet)
QuadEdge::QuadEdge(const PointSet& dPoints)
{
	//Armazenar pontos
	C = dPoints;
	/*for (int i = 0; i < C.Points.size(); i++)
	{
		Vertex v;
		v.id = i + 1;
		v.pos = C.Points[i];
		Vertices.push_back(v);
	}*/

	//Initialize
	Vertices.clear();
	Edges.clear();
	Faces.clear();
	ConvexHull.clear();
	Voronoi.clear();
}

//OVERLOAD CONSTRUCTOR (from ObjFile)
/*
QuadEdge::QuadEdge(const obj& dObj)
{
	//Armazena os nós
	Vertices.resize(dObj.Vertices.size());
	Vertices = dObj.Vertices;

	//Armazena as arestas do fecho convexo
	for (int i = 0; i < dObj.Face.size(); i++)
	{
		int j;
		if (i == dObj.Face.size() - 1)
			j = 0;
		else j = i + 1;

		Line e(dObj.Vertices[i], dObj.Vertices[j]);
		Edges.push_back(e);
	}
}
*/

//DESTRUCTOR
QuadEdge::~QuadEdge()
{
}

//INDICE (from Point)
int QuadEdge::idPoint(const Point& p)
{
	for (int i = 0; i < Vertices.size(); i++)
		if (Vertices[i].x == p.x && Vertices[i].y == p.y)
			return i + 1;
}

//BRUTE-FORCE TRIANGULATION
void QuadEdge::BruteForce()
{
	//FECHO CONVEXO
	PointSet FC(C.ConvexHull());
	//Vértices, arestas e a face do fecho
	for (int i = 0; i < FC.Points.size(); i++)
	{
		//vértices do fecho
		Vertices.push_back(FC.Points[i]);
		//Aresta do fecho
		Edge e;
		e.vFrom = i + 1;
		if (e.vFrom == FC.Points.size())
			e.vTo = 1;
		else e.vTo = e.vFrom + 1;
		Edges.push_back(e);
		//Fecho Convexo
		ConvexHull.push_back(i + 1);
		//Apaga do conjunto 'C' os vértices do Fecho
		C.DelPoint(FC.Points[i]);
	}
	//Pontos internos ao Fecho (o q restou do conjunto 'C')
	Vertices.insert(Vertices.end(), C.Points.begin(), C.Points.end());

	//TRIANGULAÇÃO
	//Parte 1 - formar arestas com pontos do fecho e armazenar os triângulos
	for (int i = 3; i < ConvexHull.size(); i++)
	{
		//Arestas
		Edge e;
		e.vFrom = 1;
		e.vTo = i;
		Edges.push_back(e);
		//Triângulos
		Face f;
		f.v.push_back(1);
		f.v.push_back(e.vTo - 1);
		f.v.push_back(e.vTo);
		Faces.push_back(f);
	}
	//Último triângulo, se tiver mais de 3 pontos
	if (Vertices.size() >= 3)
	{
		Face f;
		f.v.push_back(1);
		f.v.push_back(ConvexHull.size() - 1);
		f.v.push_back(ConvexHull.size());
		Faces.push_back(f);
	}
	//Parte 2 - para cada ponto interno, verifica em qual triângulo ele pertence e cria novas arestas
	for (int i = ConvexHull.size(); i < Vertices.size(); i++)
		for (int j = 0; j < Faces.size(); j++)
		{
			//verificar se ponto corrente está dentro da face (triângulo)
			Triangle T(Vertices[Faces[j].v[0] - 1], Vertices[Faces[j].v[1] - 1], Vertices[Faces[j].v[2] - 1]);
			if (T.Contains(Vertices[i]) == true)
			{
				//criar novas arestas
				Edge e;
				e.vFrom = i + 1; //ponto de partida da nova aresta (vértice corrente)
				for (int k = 0; k < 3; k++)
				{
					e.vTo = Faces[j].v[k]; //ponto de chegada da nova aresta = vértices da face (triângulo)
					Edges.push_back(e);
				}
				//Adicionar 3 novas faces
				Face f;
				f.v.push_back(i + 1);
				f.v.push_back(Faces[j].v[0]);
				f.v.push_back(Faces[j].v[1]);
				Faces.push_back(f);
				f.v[1] = Faces[j].v[1];
				f.v[2] = Faces[j].v[2];
				Faces.push_back(f);
				f.v[1] = Faces[j].v[2];
				f.v[2] = Faces[j].v[0];
				Faces.push_back(f);
				//Apagar face corrente
				Faces.erase(Faces.begin() + j);
				//j--;
				break;
			}
		}
}

//DELAUNAY TRIANGULATION (by Iteration algorithm O(n²))
void QuadEdge::Delaunay()
{
	//FECHO CONVEXO (apenas para ordenar os vértices e obter a primeira aresta)
	PointSet FC(C.ConvexHull());
	//Vértices, arestas e a face do fecho
	for (int i = 0; i < FC.Points.size(); i++)
	{
		//vértices do fecho
		Vertices.push_back(FC.Points[i]);
		//Fecho Convexo
		ConvexHull.push_back(i + 1);
		//Apaga do conjunto 'C' os vértices do Fecho
		C.DelPoint(FC.Points[i]);
	}
	//Pontos internos ao Fecho (o q restou do conjunto 'C')
	Vertices.insert(Vertices.end(), C.Points.begin(), C.Points.end());


	// **** PRIMEIRA ARESTA POR JARVIS **** //



	//INICIALIZAÇÃO
	//Primeira aresta
	Edge e;
	e.vFrom = 1;
	e.vTo = 2;
	e.alive = true;
	//Inserir primeira aresta na lista
	Edges.push_back(e);
	//Inicializa objeto da classe 'PointSet' com pontos do conjunto
	PointSet S(Vertices);

	//LOOP
	for (int i = 0; i < Edges.size(); i++)
	{
		//	ETAPAS:
		//	0) Verifica se a aresta corrente é 'viva'
		//	1) Verifica se tem pontos à esquerda (SIM: triangulação / NÃO: próx. aresta)
		//	2) Procura o ponto q forma um círculo vazio (cria duas novas arestas)
		//	3) Verifica se as novas arestas já existem na lista (inseri na lista a q não existir)
		
		//Verifica se a aresta é viva, senão, pula pra próxima
		if (Edges[i].alive == false)
			continue;
		//Linha da aresta corrente
		Line currentLine(Vertices[Edges[i].vFrom - 1], Vertices[Edges[i].vTo - 1]);
		//Verifica se tem pontos à esquerda da aresta corrente
		PointSet LeftPointsSet(S.LeftPoints(currentLine));
		if (LeftPointsSet.Points.size() > 0)
		{
			//Procura o ponto q forma um círculo vazio
			for (int j = 0; j < LeftPointsSet.Points.size(); j++) //'n' no começo mas depois vai diminuindo
			{
				//Triângulo formado pela aresta corrente e o ponto 'teste'
				Triangle t(Vertices[Edges[i].vFrom - 1], Vertices[Edges[i].vTo - 1], LeftPointsSet.Points[j]);
				//Verifica se existe algum ponto dentro da circunferência
				for (int k = 0; k < LeftPointsSet.Points.size(); k++) //'n' no começo tbm...
				{
					if (t.InCircle(LeftPointsSet.Points[k]) == true)
						break; //se tiver ponto dentro, pula pro próximo triângulo
					else if (k == LeftPointsSet.Points.size() - 1)
					{

						//Caso o círculo com triângulo inscrito esteja vazio, criar novas arestas
						//Aresta 1
						e.vFrom = Edges[i].vFrom;
						e.vTo = idPoint(LeftPointsSet.Points[j]); //índice do ponto em análise
						e.alive = true;

						//Verifica se a aresta já existe
						/*for (int n = 0; n < Edges.size(); n++)
							if (Edges[n].vFrom == e.vTo && Edges[n].vTo == e.vFrom)
							{
								Edges[n].alive = false;
								break;
							}
							else if (Edges[n].vFrom == e.vFrom && Edges[n].vTo == e.vTo)
							{
								Edges[n].alive = false;
								break;
							}
							else if (n == Edges.size() - 1)
							{
								Edges.push_back(e);
								break;
							}*/

						//Verifica se a aresta já existe (ou se cruza outra aresta)
						for (int n = 0; n < Edges.size(); n++)
						{
							Line newEdge(Vertices[e.vFrom - 1], Vertices[e.vTo - 1]);
							Line existentEdge(Vertices[Edges[n].vFrom - 1], Vertices[Edges[n].vTo - 1]);
							if (newEdge == existentEdge)
							{
								Edges[n].alive = false;
								break;
							}
							else if (newEdge.LineLine(existentEdge) == true)
							{
								//Edges[n].alive = false;
								break;
							}
							else if (n == Edges.size() - 1)
							{
								Edges.push_back(e);
								break;
							}
						}

						//Aresta 2
						e.vFrom = idPoint(LeftPointsSet.Points[j]); //índice do ponto em análise
						e.vTo = Edges[i].vTo;
						e.alive = true;

						//Verifica se a aresta já existe
						/*for (int n = 0; n < Edges.size(); n++)
							if (Edges[n].vFrom == e.vTo && Edges[n].vTo == e.vFrom)
							{
								Edges[n].alive = false;
								break;
							}
							else if (Edges[n].vFrom == e.vFrom && Edges[n].vTo == e.vTo)
							{
								Edges[n].alive = false;
								break;
							}
							else if (n == Edges.size() - 1)
							{
								Edges.push_back(e);
								break;
							}*/

						//Verifica se a aresta já existe (ou se cruza outra aresta)
						for (int n = 0; n < Edges.size(); n++)
						{
							Line newEdge(Vertices[e.vFrom - 1], Vertices[e.vTo - 1]);
							Line existentEdge(Vertices[Edges[n].vFrom - 1], Vertices[Edges[n].vTo - 1]);
							if (newEdge == existentEdge)
							{
								Edges[n].alive = false;
								break;
							}
							else if (newEdge.LineLine(existentEdge) == true)
							{
								//Edges[n].alive = false;
								break;
							}
							else if (n == Edges.size() - 1)
							{
								Edges.push_back(e);
								break;
							}
						}
							
						//Mata a aresta base
						Edges[i].alive = false;
						//Para sair do loop q procura pontos ótimos (triângulos com círculos vazios) 
						j = LeftPointsSet.Points.size();

                        // ********* VORONOI **********
                        //Armazenar centro do circulo formado pelo triangulo corrente
                        Point cc(t.CircleCenter());
                        Voronoi.push_back(cc);
					}
				}
			}
		}
		else
		{
			Edges[i].alive = false;
			continue;
		}
	} //end LOOP

	//RESUMO
	cout << "\n\tNumber of edges: " << Edges.size() << endl;

    //DEBUG
    cout << "\tPela formaula: 3*n-v-3 = " << 3*Vertices.size()-FC.Points.size()-3 << endl;
	
	//CORRIGIR VORONOI
	/*cout << "\tTamanho do vetor de pontos de Voronoi: " << Voronoi.size() << endl;
	cout << "\tPela formaula: 2*n-v-2 = " << 2 * Vertices.size() - FC.Points.size() - 2 << endl;*/
}


//DELAUNAY TRIANGULATION (by Divide-and-Conquer algorithm O(n logn) )
void QuadEdge::Delaunay2() const
{

}
