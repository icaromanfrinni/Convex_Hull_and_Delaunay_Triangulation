#include "icaroView.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//FUNCTION DECLARATIONS
//Ações do teclado
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

//VARIABLE DECLARATIONS
float scaleFactor = 1.0;
int Xo = 0;
int Yo = 0;
int iDelay = 0;
bool Voronoi_KEY = false;
bool Spline_KEY = false;

//DEFAULT CONSTRUCTOR
icaroView::icaroView()
{
	vPt2D.clear();
	vLine2D.clear();
	vFace2D.clear();
	vPtVoronoi2D.clear();
}

//OVERLOAD CONSTRUCTOR
icaroView::icaroView(const ObjFile& inObjFile)
{
	vPt2D.clear();
	vLine2D.clear();
	vFace2D.clear();
	vPtVoronoi2D.clear();

	//int iCount = 0; //contagem dos vértices já acessados para reiniciar o índice dos vértices do objeto corrente
	for (int i = 0; i < inObjFile.vObj.size(); i++)
	{
		//PONTOS
		for (int j = 0; j < inObjFile.vObj[i].Vertices.size(); j++)
		{
			Pt2D p2D;
			p2D.x = inObjFile.vObj[i].Vertices[j].x;
			p2D.y = inObjFile.vObj[i].Vertices[j].y;
			vPt2D.push_back(p2D);
		}
		//PONTOS DE VORONOI
		for (int j = 0; j < inObjFile.vObj[i].Voronoi.size(); j++)
		{
			Pt2D pV2D;
			pV2D.x = inObjFile.vObj[i].Voronoi[j].x;
			pV2D.y = inObjFile.vObj[i].Voronoi[j].y;
			vPtVoronoi2D.push_back(pV2D);
		}
		//ARESTAS
		for (int j = 0; j < inObjFile.vObj[i].Lines.size(); j++)
		{
			Line2D e2D;
			e2D.pFrom.x = vPt2D[inObjFile.vObj[i].Lines[j].vFrom - 1].x;
			e2D.pFrom.y = vPt2D[inObjFile.vObj[i].Lines[j].vFrom - 1].y;
			e2D.pTo.x = vPt2D[inObjFile.vObj[i].Lines[j].vTo - 1].x;
			e2D.pTo.y = vPt2D[inObjFile.vObj[i].Lines[j].vTo - 1].y;
			vLine2D.push_back(e2D);
		}
		//FACE
		Face2D f;
		for (int j = 0; j < inObjFile.vObj[i].Face.size(); j++)
		{
			Pt2D p2D;
			p2D.x = vPt2D[inObjFile.vObj[i].Face[j] - 1].x;
			p2D.y = vPt2D[inObjFile.vObj[i].Face[j] - 1].y;
			f.v.push_back(p2D);
		}
		vFace2D.push_back(f);
		//iCount += inObjFile.vObj[i].Vertices.size();
	}
}

//DESTRUCTOR
icaroView::~icaroView()
{
}

//ALLOCATION (EDGES)
//void icaroView::SetEdges(const ObjFile& inObjFile)
//{
//	//Inicializa o vetor de faces com a quantidade de Objetos
//	vPoly2D.clear();
//	vPoly2D.resize(inObjFile.vObj.size());
//
//	for (int i = 0; i < inObjFile.vObj.size(); i++)
//	{
//		//VARIÁVEIS tipo 'float' com coordenadas 'x' e 'y'
//		for (int j = 0; j < inObjFile.vObj[i].Vertices.size(); j++)
//		{
//			Pt2D point2D;
//			point2D.x = inObjFile.vObj[i].Vertices[j].x;
//			point2D.y = inObjFile.vObj[i].Vertices[j].y;
//			vPoly2D[i].vVtx2D.push_back(point2D);
//		}
//	}
//}

//INITIALIZE OPENGL WINDOW
void icaroView::DrawOpenGL()
{
	//Inicializa a biblioteca GLFW
	if (!glfwInit())
	{
		cerr << "\n\t!!! FAILED TO INITIALIZE 'GLFW' !!!\n" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	//Criar uma janela em OpenGL
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "DELAUNAY TRIANGULATION", NULL, NULL);

	//Retorna o tamanho do framebuffer da janela atual
	//int screenWidth, screenHeight;
	//glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Se não iniciar a janela corretamente
	if (!window)
	{
		//Finalizar
		glfwTerminate();
		cerr << "\n\t!!! FAILED TO OPEN 'GLFW' WINDOW !!!\n" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	//Tornar essa janela atual
	glfwMakeContextCurrent(window);

	//Notificar quando uma tecla for pressionada
	glfwSetKeyCallback(window, keyCallback);
	//Modo de entrada da tecla
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

#pragma region SISTEMA DE COORDENADAS
	//Especifica a parte da janela que o OpenGL vai desenhar (em pixels)
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//Matriz de projeção define as propriedades da câmera que exibe os objetos no quadro de coordenadas mundiais. Aqui, você normalmente define o fator de zoom, a proporção e os planos de recorte próximos e distantes.
	glMatrixMode(GL_PROJECTION);
	//Substitui a Matriz atual pela matriz Identidade e inicia uma nova, pq as matrizes glOrtho e glRotate acumulam [basicamente nos coloca em (0,0,0)].
	glLoadIdentity();
	//Essencialemte defini o sistema de coordenadas
	glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
	//Modo Matriz Padrão (define como os objetos são transformados (rotação, translação) no mundo
	glMatrixMode(GL_MODELVIEW);
	//Substitui a Matriz atual pela matriz Identidade
	glLoadIdentity();
#pragma endregion

	//Loop até o usuário fechar essa janela
	while (!glfwWindowShouldClose(window))
	{
		//Limpar todos os pixels
		glClear(GL_COLOR_BUFFER_BIT);
		//Expessura da linha
		glLineWidth(1);
		//Habilitar BLEND
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//Cor da tela de fundo
		glShadeModel(GL_SMOOTH);
		glClearColor(0.6, 0.6, 0.6, 0.0);
		
		//SPLINE 'TEST'
		/*glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, vPt2D.size(), vPt2D.size() + 1, &vPt2D[0].x);
		glEnable(GL_MAP1_VERTEX_3);*/

#pragma region RENDERIZAR

		//Redesenhar quando alterar tamanho da janela
		Reshape();

		/* FACES */
		DrawFaces();

		/* POINTS */
		DrawPoints();

		/* SPLINE */
		DrawSpline();

		/* EDGES */
		DrawEdges();

		/* POINTS OF VORONOI */
		DrawVoronoiPoints();
		
#pragma endregion

		//Desabilitar BLEND
		glDisable(GL_BLEND);
		//trocar buffers (mandar o da frente pra trás e o de trás pra frente)
		glfwSwapBuffers(window);
		//Procurar e processar eventos
		glfwPollEvents();
		//DELAY
		this_thread::sleep_for(chrono::milliseconds(40));
		//this_thread::sleep_for(chrono::seconds(1));
	}
	//REINICIAR CONTADOR
	iDelay = 0;
	//REINICIAR FATOR DE ESCALA
	scaleFactor = 1.0;
	//REINICIAR Coord da Origem
	Xo = 0;
	Yo = 0;
	//Finalizar
	glfwTerminate();
}

//RESHAPE
void icaroView::Reshape()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	//SCALE
	float newWidth, newHeight;
	newWidth = width * scaleFactor + Xo;
	newHeight = height * scaleFactor + Yo;

	//Especifica a parte da janela que o OpenGL vai desenhar (em pixels)
	glViewport(0, 0, width, height);
	//Matriz de projeção define as propriedades da câmera que exibe os objetos no quadro de coordenadas mundiais. Aqui, você normalmente define o fator de zoom, a proporção e os planos de recorte próximos e distantes.
	glMatrixMode(GL_PROJECTION);
	//Substitui a Matriz atual pela matriz Identidade e inicia uma nova, pq as matrizes glOrtho e glRotate acumulam [basicamente nos coloca em (0,0,0)].
	glLoadIdentity();
	//Essencialemte defini o sistema de coordenadas
	glOrtho(Xo, newWidth, Yo, newHeight, 0, 1);

	//GRADE (desenha linhas da grade de fundo)
	int nHeight = height / 2; //número de linhas horizontais
	int nWidth = width / 2; //número de linhas verticais
	glColor3f(0.5, 0.5, 0.5); //color
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINES);
	for (int i = -nHeight; i < nHeight; i++) //Linhas horizontais
	{
		glVertex2f(Xo, i * 50);
		glVertex2f(newWidth, i * 50);
	}
	for (int i = -nWidth; i < nWidth; i++) //Linhas Verticais
	{
		glVertex2f(i * 50, Yo);
		glVertex2f(i * 50, newHeight);
	}
	glEnd(); //End of GL_LINES
	glDisable(GL_LINE_SMOOTH);
}

//DRAW POINTS
void icaroView::DrawPoints()
{
	glEnable(GL_POINT_SMOOTH);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i = 0; i < vPt2D.size(); i++)
		glVertex2f(vPt2D[i].x, vPt2D[i].y);
	glEnd();
}

//DRAW SPLINE
void icaroView::DrawSpline()
{
	if (Spline_KEY == true)
	{
		/*glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 30; i++)
			glEvalCoord1f((GLfloat)i / 30);
		glEnd();*/

		/* PONTOS DE CONTROLE */
		glColor3f(1.0, 1.0, 0.0);
		glPointSize(5);
		glBegin(GL_POINTS);
		for (int i = 0; i < vPt2D.size(); i++)
			glVertex2f(vPt2D[i].x, vPt2D[i].y);
		glEnd();
	}
}

//DRAW VORONOI POINTS
void icaroView::DrawVoronoiPoints()
{
	if (Voronoi_KEY == true)
	{
		glEnable(GL_POINT_SMOOTH);
		glColor3f(1.0, 0.0, 0.0);
		glPointSize(5);
		glBegin(GL_POINTS);
		for (int i = 0; i < vPtVoronoi2D.size(); i++)
			glVertex2f(vPtVoronoi2D[i].x, vPtVoronoi2D[i].y);
		glEnd();
	}
}

//DRAW EDGES
void icaroView::DrawEdges()
{
	//EDGES
	for (int i = 0; i < iDelay; i++)
	{
		//Última aresta tem cor vermelha (o restante é preto)
		if (i == iDelay - 1)
			glColor3f(1.0, 0.0, 0.0); //red
		else glColor3f(0.0, 0.0, 0.0); //black
		//glEnable(GL_LINE_SMOOTH);
		glBegin(GL_LINES);
			glVertex2f(vLine2D[i].pFrom.x, vLine2D[i].pFrom.y);
			glVertex2f(vLine2D[i].pTo.x, vLine2D[i].pTo.y);
		glEnd();
	}
	
	//Contador
	iDelay++;
	if (iDelay > vLine2D.size())
		iDelay--;

	/*
	//EDGES (CÓDIGO PARA DESENHAR ARESTAS DE UMA VEZ, OU SEJA, SEM DELAY)
	for (int i = 0; i < vLine2D.size(); i++)
	{
		glColor3f(0.0, 0.0, 0.0); //color
		//glEnable(GL_LINE_SMOOTH);
		glBegin(GL_LINES);
			glVertex2f(vLine2D[i].pFrom.x, vLine2D[i].pFrom.y);
			glVertex2f(vLine2D[i].pTo.x, vLine2D[i].pTo.y);
		glEnd();
	}
	*/
}

//DRAW FACES
void icaroView::DrawFaces()
{
	//FACE
	glColor4f(0.0, 0.5, 0.0, 0.5); //color
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //desenhar apenas as arestas
	for (int i = 0; i < vFace2D.size(); i++)
	{
		glBegin(GL_POLYGON);
		for (int j = 0; j < vFace2D[i].v.size(); j++)
			glVertex2f(vFace2D[i].v[j].x, vFace2D[i].v[j].y);
		glEnd();
	}
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	cout << key << endl;

	if (action == GLFW_RELEASE)
		return;

	//actions are GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
	/*
	if (key == GLFW_KEY_SPACE && action == GLFW_REPEAT)
	{
		cout << "space key pressed" << endl;
	}
	*/

	//Zoom In
	if (key == 61)
	{
		scaleFactor -= 0.1;
		cout << "Zoom IN" << endl;
	}

	//Zoom Out
	if (key == 45)
	{
		scaleFactor += 0.1;
		cout << "Zoom OUT" << endl;
	}

	//PanX (RIGHT)
	if (key == 262) //Tecla 'Seta para DIREITA'
	{
		Xo += 10;
		cout << "Pan RIGHT" << endl;
	}

	//PanX (LEFT)
	if (key == 263) //Tecla 'Seta para ESQUERDA'
	{
		Xo -= 10;
		cout << "Pan LEFT" << endl;
	}

	//PanY (UP)
	if (key == 265) //Tecla 'Seta para CIMA'
	{
		Yo += 10;
		cout << "Pan UP" << endl;
	}

	//PanY (DOWN)
	if (key == 264) //Tecla 'Seta para BAIXO'
	{
		Yo -= 10;
		cout << "Pan DOWN" << endl;
	}

	//Voronoi diagram
	if (key == 86) //Tecla 'V'
		if (Voronoi_KEY == true)
		{
			Voronoi_KEY = false;
			cout << "Voronoi TURN OFF" << endl;
		}
		else
		{
			Voronoi_KEY = true;
			cout << "Voronoi TURN ON" << endl;
		}

	//Spline
	if (key == 83) //Tecla 'S'
		if (Spline_KEY == true)
		{
			Spline_KEY = false;
			cout << "Spline TURN OFF" << endl;
		}
		else
		{
			Spline_KEY = true;
			cout << "Spline TURN ON" << endl;
		}
}