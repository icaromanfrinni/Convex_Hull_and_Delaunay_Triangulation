#include <iostream>
#include <random>
#include <string>
#include "icarolib.h"
#include "ObjFile.h"

using namespace std;

//------------------------------------------------------------------------------
//FUNCTION DECLARATIONS
//------------------------------------------------------------------------------

//SCREEN
void clearScreen();
void pauseScreen();

void MainMenu();
void Random();
void RandomDelaunay();
void ObjConvexHull();
void JoinObjConvexHull();
void ObjTriangulation();
void OpenObjFile();

//------------------------------------------------------------------------------
//MAIN FUNCTION
//------------------------------------------------------------------------------

int main()
{
	while (true)
		MainMenu();

	return 0;
}

//------------------------------------------------------------------------------
//FUNCTION DEFINITIONS
//------------------------------------------------------------------------------

#pragma region SCREEN
//Clear
void clearScreen()
{
#ifdef __unix__
	system("clear");
#elif defined _WIN32
	system("cls");
#endif
}

//Pause
void pauseScreen()
{
#ifdef __unix__
	system("read -p '\tPress Enter to continue...' var");
#elif defined _WIN32
	system("pause");
#endif
}
#pragma endregion

void MainMenu()
{
	clearScreen();

	int option = 0;

	//MAIN MENU
	cout << "\n";
	cout << "\t==============================================================\n";
	cout << "\t==                                                          ==\n";
	cout << "\t==                  DELAUNAY TRIANGULATION                  ==\n";
	cout << "\t==         (with Convex Hull by QuickHull algorithm)        ==\n";
	cout << "\t==                                                          ==\n";
	cout << "\t==              MS and PhD in Computer Science              ==\n";
	cout << "\t==               Federal University of Ceara                ==\n";
	cout << "\t==                       (MDCC/UFC)                         ==\n";
	cout << "\t==                                                          ==\n";
	cout << "\t==============================================================\n";
	cout << "\n";
	cout << "\tMAIN MENU\n";
	cout << "\n";
	cout << "\t1. Brute-Force (from Random Points)\n";
	cout << "\t2. Delaunay (from Random Points)\n";
	cout << "\t3. Convex Hull (from OBJ file)\n";
	cout << "\t4. Triangulation (from OBJ file)\n";
	cout << "\t5. Open OBJ file\n";
	cout << "\t0. EXIT\n";
	cout << "\n";
	cout << "\tEnter your choice: ";
	cin >> option;
	cout << endl;
	switch (option)
	{
	case 1:
		Random();
		break;
	case 2:
		RandomDelaunay();
		break;
	case 3:
		ObjConvexHull();
		break;
	case 4:
		ObjTriangulation();
		break;
	case 5:
		OpenObjFile();
		break;
	case 0:
		exit(0);
		break;
	default:
		cerr << "\n\t! INVALID OPTION !" << endl;
		pauseScreen();
		break;
	}
}

void Random()
{
	int n;
	cout << "\n\tEnter the number of points: ";
	cin >> n;

	random_device randomGen;
	uniform_int_distribution<int> intDistr(100, 500);

	PointSet C;
	//Criar pontos aleatórios
	for (int i = 0; i < n; i++)
	{
		int x = intDistr(randomGen);
		int y = intDistr(randomGen);
		/*bool check = false;
		for (int j = 0; j < C.Points.size(); j++)
			if (C.Points[j].x == x || C.Points[j].y == y)
			{
				check = true;
				i--;
				break;
			}
		if (check == false)
		{*/
			Point pt(x, y);
			C.Points.push_back(pt);
		/*}*/
	}

	//Deletar pontos repetidos
	C.DelRepeatPoint();

	//Triangulação
	QuadEdge T(C);
	T.BruteForce();

	//Objeto do arquivo .OBJ
	ObjFile file(T);
	//Escrever arquivo de saída
	file.WriteObjFile();

	//Desenhar arquivo final
	icaroView objView(file);
	objView.DrawOpenGL();

	pauseScreen();
}

void RandomDelaunay()
{
	int n;
	cout << "\n\tEnter the number of points: ";
	cin >> n;

	random_device randomGen;
	uniform_int_distribution<int> intDistr(100, 500);

	PointSet C;
	//Criar pontos aleatórios
	for (int i = 0; i < n; i++)
	{
		int x = intDistr(randomGen);
		int y = intDistr(randomGen);
		/*bool check = false;
		for (int j = 0; j < C.Points.size(); j++)
			if (C.Points[j].x == x || C.Points[j].y == y)
			{
				check = true;
				i--;
				break;
			}
		if (check == false)
		{*/
		Point pt(x, y);
		C.Points.push_back(pt);
		/*}*/
	}

	//Deletar pontos repetidos
	C.DelRepeatPoint();

	//Triangulação
	QuadEdge T(C);
	T.Delaunay();

	//Objeto do arquivo .OBJ
	ObjFile file(T);
	//Escrever arquivo de saída
	file.WriteObjFile();

	//Desenhar arquivo final
	icaroView objView(file);
	objView.DrawOpenGL();

	pauseScreen();
}


void ObjConvexHull()
{
	ObjFile inputFile;
	//Ler arquivo de entrada
	if (inputFile.ReadObjFile() == true)
	{
		//Contador de vértices do fecho
		int vCount = 0;
		//Para cada objeto do arquivo de entrada...
		for (int i = 0; i < inputFile.vObj.size(); i++)
		{
			//Inicializa conjunto 'C' com pontos do objeto corrente
			PointSet C(inputFile.vObj[i].Vertices);
			//Inicializa conjunto 'FC' com pontos do Fecho Convexo
			PointSet FC(C.ConvexHull());
			//Limpar vetor de pontos e vértices da face do objeto corrente
			inputFile.vObj[i].Vertices.clear();
			inputFile.vObj[i].Face.clear();
			inputFile.vObj[i].Lines.clear();
			//Devolver vértices do fecho para o objeto corrente
			for (int j = 0; j < FC.Points.size(); j++)
			{
				inputFile.vObj[i].Vertices.push_back(FC.Points[j]);
				vCount++; //contador de vértices contínuo
				inputFile.vObj[i].Face.push_back(vCount);
				//Aresta do fecho
				objLine e;
				e.vFrom = vCount;
				if (j == FC.Points.size() - 1)
					e.vTo = vCount - j;
				else e.vTo = e.vFrom + 1;
				inputFile.vObj[i].Lines.push_back(e);
				//Apaga do conjunto 'C' os vértices do Fecho
				C.DelPoint(FC.Points[j]);
			}
			//Resgatar pontos internos ao Fecho (o q restou do conjunto 'C')
			inputFile.vObj[i].Vertices.insert(inputFile.vObj[i].Vertices.end(), C.Points.begin(), C.Points.end());
			//Somar ao contador a quantidade de pontos internos
			vCount += C.Points.size();

			/*
				Dessa forma, os pontos do fecho ficam
				no começo da listagem e continuam ordenados! ;P
			*/
		}

		//Escrever arquivo de saída
		inputFile.WriteObjFile();

		//Desenhar arquivo final
		icaroView objView(inputFile);
		objView.DrawOpenGL();
	}
	pauseScreen();
}

//NÃO FUNCIONANDO POR ENQUANTO =]
void JoinObjConvexHull()
{
	ObjFile inputFile;
	//Ler arquivo de entrada
	if (inputFile.ReadObjFile() == true)
	{
		//Para cada objeto do arquivo de entrada...
		for (int i = 0; i < inputFile.vObj.size(); i++)
		{
			//Inicializa conjunto 'C' com pontos do objeto corrente
			PointSet C(inputFile.vObj[i].Vertices);
			//Inicializa conjunto 'FC' com pontos do Fecho Convexo
			PointSet FC(C.ConvexHull());
			//Limpar vetor de pontos e vértices da face do objeto corrente
			inputFile.vObj[i].Vertices.clear();
			//Devolver vértices do fecho para o objeto corrente
			for (int j = 0; j < FC.Points.size(); j++)
			{
				inputFile.vObj[i].Vertices.push_back(FC.Points[j]);
				//Apaga do conjunto 'C' os vértices do Fecho
				C.DelPoint(FC.Points[j]);
			}
			//Resgatar pontos internos ao Fecho (o q restou do conjunto 'C')
			inputFile.vObj[i].Vertices.insert(inputFile.vObj[i].Vertices.end(), C.Points.begin(), C.Points.end());
			//Atualiza tamanho do vetor Face pra receber os nós do fecho depois da união de polígonos adjacentes
			inputFile.vObj[i].Face.resize(FC.Points.size());
		}

		/* ANTES: Inicializava aqui pra desenhar as faces dos fechos separadas */
		/* Lá no final, desenhava as arestas do conjunto de pontos final, depois das uniões */
		//Inicializar objeto para desenhar faces dos fechos
		//icaroView objView(inputFile);

		//Comparar objetos && Eliminar arestas coincidentes && Unir objetos vizinhos
		for (int i = 0; i < inputFile.vObj.size(); i++)
			for (int j = i + 1; j < inputFile.vObj.size(); j++)
			{
				//Inicializar os dois polígonos
				Polygon P1(inputFile.vObj[i]);
				Polygon P2(inputFile.vObj[j]);
				//Verificar se estão próximos
				if (P1.NextTo(P2) == true)
				{
					Polygon U(P1.Merge(P2));

					//Adiciona no U os pontos internos de P1
					//(pega os pontos q estão depois do tamanho do vetor Face)
					U.Vertices.insert(U.Vertices.end(), inputFile.vObj[i].Vertices.begin() + inputFile.vObj[i].Face.size(), inputFile.vObj[i].Vertices.end());
					//Adiciona no U os pontos internos de P2
					//(pega os pontos q estão depois do tamanho do vetor Face)
					U.Vertices.insert(U.Vertices.end(), inputFile.vObj[j].Vertices.begin() + inputFile.vObj[j].Face.size(), inputFile.vObj[j].Vertices.end());

					//armazenar U no lugar de P1
					inputFile.vObj[i].Vertices = U.Vertices;
					//atualiza tamanho do vetor Face com a soma dos dois fechos
					inputFile.vObj[i].Face.resize(inputFile.vObj[i].Face.size() + inputFile.vObj[j].Face.size() - 2);

					//apagar objeto P2
					inputFile.vObj.erase(inputFile.vObj.begin() + j);
					//volta uma casa
					j--;
				}
			}

		//Armazenar índice dos vértices das faces no padrão .OBJ
		int vCount = 1; //Inicializar contador de vértices
		for (int i = 0; i < inputFile.vObj.size(); i++)
		{
			for (int j = 0; j < inputFile.vObj[i].Face.size(); j++)
				inputFile.vObj[i].Face[j] = vCount + j;

			vCount += inputFile.vObj[i].Vertices.size();
		}

		//Escrever arquivo de saída
		inputFile.WriteObjFile();

		//Inicializar objeto para desenhar faces dos fechos
		icaroView objView(inputFile);

		//Desenhar arquivo final
		//objView.SetEdges(inputFile);
		objView.DrawOpenGL();
	}

	pauseScreen();
}

void ObjTriangulation()
{
	ObjFile inputFile;

	if (inputFile.ReadObjFile() == true)
	{
		//Contador de vértices do fecho
		int vFaceCount = 0;
		//Contador de arestas da triangulação
		int vEdgeCount = 0;

		for (int i = 0; i < inputFile.vObj.size(); i++)
		{
			//Pontos do Obj corrente
			PointSet C(inputFile.vObj[i].Vertices);

			//Triangulação
			QuadEdge T(C);
			T.Delaunay();

			//Retornar pontos ordenados para a classe Obj File
			inputFile.vObj[i].Vertices = T.Vertices;

			//Arestas da triangulação
			inputFile.vObj[i].Lines.clear();
			for (int j = 0; j < T.Edges.size(); j++)
			{
				objLine e;
				e.vFrom = T.Edges[j].vFrom + vEdgeCount;
				e.vTo = T.Edges[j].vTo + vEdgeCount;
				inputFile.vObj[i].Lines.push_back(e);
			}
			vEdgeCount += T.Vertices.size();

			//Face do Fecho Convexo
			inputFile.vObj[i].Face.clear();
			for (int j = 0; j < T.ConvexHull.size(); j++)
			{
				vFaceCount++;
				inputFile.vObj[i].Face.push_back(vFaceCount);
			}

			//Pontos de Voronoi
			inputFile.vObj[i].Voronoi = T.Voronoi;

			//Quantidade de pontos internos
			int inPoints = T.Vertices.size() - T.ConvexHull.size();
			vFaceCount += inPoints;
		}

		inputFile.WriteObjFile();

		icaroView objView(inputFile);
		objView.DrawOpenGL();
	}
	pauseScreen();
}

void OpenObjFile()
{
	ObjFile inputFile;
	//Ler arquivo de entrada
	if (inputFile.ReadObjFile() == true)
	{
		//Desenhar arquivo
		icaroView objView(inputFile);
		objView.DrawOpenGL();
	}

	pauseScreen();
}
