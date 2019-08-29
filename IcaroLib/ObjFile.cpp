#include "ObjFile.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

//DEFAULT CONSTRUCTOR
ObjFile::ObjFile()
{
	object = false;
	//fName = ""; //"Pensei q podia dar problema na hora de escrever o arquivo de pontos randômicos"
	ClearCurrentObj();
	vObj.clear();
}

//DEFAULT CONSTRUCTOR
ObjFile::ObjFile(const QuadEdge& qEdge)
{
	ClearCurrentObj();
	cObj.Name = "random";
	cObj.Vertices = qEdge.Vertices;
	for (int i = 0; i < qEdge.Edges.size(); i++)
	{
		objLine l;
		l.vFrom = qEdge.Edges[i].vFrom;
		l.vTo = qEdge.Edges[i].vTo;
		cObj.Lines.push_back(l);
	}
	cObj.Face = qEdge.ConvexHull;
	cObj.Voronoi = qEdge.Voronoi;

	vObj.push_back(cObj);
}

//DESTRUCTOR
ObjFile::~ObjFile()
{
}

//CLEAR OBJ VARIABLE TYPE
void ObjFile::ClearCurrentObj()
{
	cObj.Name = "";
	cObj.Vertices.clear();
	cObj.Lines.clear();
	cObj.Face.clear();
	cObj.Voronoi.clear();
}

//READ .OBJ FILE
bool ObjFile::ReadObjFile()
{
	cout << "\n\tEnter the input file name [.obj]: ";
	cin >> fName;
	string inName = fName + ".obj";

	ifstream inFile(inName);
	if (!inFile.is_open())
	{
		cerr << "\n\t!!! FILE COULD NOT BE OPENED !!!\n" << endl;
		return false;
	}

	//Read a file line by line
	string line;
	ClearCurrentObj();
	object = false;

	while (!inFile.eof())
	{
		getline(inFile, line);
		
		/*-------------------* OBJECT *-------------------*/

		if (line.substr(0, 1) == "o")
		{
			//Objeto anterior, se existir
			//e limpa a variável q recebe o objeto corrente
			if (object == true)
			{
				vObj.push_back(cObj);
				ClearCurrentObj();
			}

			object = true;					//ativa a leitura de um novo objeto
			cObj.Name = line.substr(2);		//recebe o nome do objeto
			cout << "\n\tReading Object ...: " << cObj.Name << endl;
		}
		else if (object == true)
		{

			/*-------------------* VERTEX *-------------------*/

			if (line.substr(0, 1) == "v")
			{
				Point vertex;
				line = line.substr(1);
				istringstream s(line);
				s >> vertex.x;
				s >> vertex.y;
				cObj.Vertices.push_back(vertex);
			}
			
			/*--------------------* LINE *--------------------*/

			if (line.substr(0, 1) == "l")
			{
				objLine vertex;
				line = line.substr(1);
				istringstream s(line);
				s >> vertex.vFrom;
				s >> vertex.vTo;
				cObj.Lines.push_back(vertex);
			}
			
			/*--------------------* FACE *--------------------*/

			if (line.substr(0, 1) == "f")
			{
				int vertex;
				line = line.substr(1);				
				istringstream s(line);

				while (true)
				{
					s >> vertex;
					if (!s)
						break;
					cObj.Face.push_back(vertex);
				}
			}
		}
	}
	//END OF FILE

	//Recebe último objeto válido
	if (object == true)
		vObj.push_back(cObj);

	cout << "\n\tFile [" << inName << "] has been opened successfully!" << endl;

	return true;
}

//WRITE .OBJ FILE
void ObjFile::WriteObjFile()
{
	string outName = fName + "Out.obj";
	ofstream outFile(outName);
	if (!outFile.is_open())
	{
		cerr << "\n\t!!! FILE COULD NOT BE OPENED !!!\n" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	/*-------------------* HEADER *-------------------*/

	outFile << "# ICARO 2019 OBJ File" << endl;
	outFile << "# icaro@lia.ufc.br" << endl;

	/*------------------* OBJECTS *-------------------*/

	int vCount = 0;
	for (int i = 0; i < vObj.size(); i++)
	{
		//group name (o)
		outFile << "o " << vObj[i].Name << endl;

		//geometric vertices (v)
		for (int j = 0; j < vObj[i].Vertices.size(); j++)
			outFile << "v " << vObj[i].Vertices[j].x << " " << vObj[i].Vertices[j].y << " " << "0.00" << endl;
		outFile << "# " << vObj[i].Vertices.size() << " vertices" << endl;

		//geometric lines (l)
		for (int j = 0; j < vObj[i].Lines.size(); j++)
			outFile << "l " << vObj[i].Lines[j].vFrom << " " << vObj[i].Lines[j].vTo << endl;
		outFile << "# " << vObj[i].Lines.size() << " lines" << endl;

		//face vertices (f)
		outFile << "f";
		for (int j = 0; j < vObj[i].Face.size(); j++)
			outFile << " " << vObj[i].Face[j];
		outFile << endl;
	}

	/*--------------------* END *--------------------*/

	outFile.close();

	cout << "\n\tFile [" << outName << "] has been created successfully!" << endl;
}
