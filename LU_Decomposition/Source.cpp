#include<iostream>
#include<string.h>
#include<string>
#include<sstream>
#include<fstream>

static std::string outputFile = "outputFile.txt";
static std::string inputFile = "inputFile.txt";
static std::ofstream out;
static std::ifstream in;
template <typename T = std::string, typename U = int>
struct KeyValuePair {
	T* key;
	U* value;
	int size;



	KeyValuePair(int size) {
		this->size = size;

		key = new T[size]{};
		value = new U[size]{};
	}

	~KeyValuePair() {
		delete[] key;
		delete[] value;

		key = nullptr;
		value = nullptr;
	}

	void display(int matrixOrder, int condition) const {//condition = 0 --> display keys	condition = 1 --> display values	condition = 2 --> display Both 

		if (condition == 0 || condition == 2) {
			for (int i = 0; i < size; i++) {
				if (i % matrixOrder == 0 && i > 0) {
					out << "\n";
				}
				out << key[i] << "   ";

			}
			out << "\n";
		}

		if (condition == 2) {
			out << "\n\n";
		}
		if (condition == 1 || condition == 2) {
			for (int i = 0; i < size; i++) {
				if (i % matrixOrder == 0 && i > 0) {
					out << "\n";
				}
				out << value[i] << "  ";

			}
			out << "\n\n";
		}
	}
};

template<typename T = int>
struct List {
	T* list;
	int size;

	List() {
		list = NULL;
		size = 0;
	}

	List(int size) {
		this->size = size;
		list = new T[size]{};
	}

	void initializeList(int size) {
		this->size = size;
		list = new T[size]{};
	}

	void display(int matrixOrder) const {

		for (int i = 0; i < size; i++) {
			if (i % matrixOrder == 0 && i > 0) {
				out << "\n";
			}
			out << list[i] << "   ";

		}
		out << "\n\n";
	}

	~List() {
		if (list != NULL) {
			delete[] list;
			list = nullptr;
		}
	}
};

//Input for matrix A and B
void takeInput(const List<float>& A, const List<float>& B) {
	std::cout << "For AX = B\n";
	std::cout << "Enter elements for A\n";
	for (int i = 0; i < A.size; i++) {
		std::cout << i + 1 << ": ";
		std::cin >> A.list[i];
	}
	std::cout << "\nEnter elements for B\n";
	for (int i = 0; i < B.size; i++) {
		std::cout << i + 1 << ": ";
		std::cin >> B.list[i];
	}
}

//helper method to find index by the use of string because the length of string is 3, morover 2nd and 3rd char is number (which in findLandUMatrix(...) method)
//this method is used to find values for L and U
int findIndex(std::string& temp, int orderOfMatrix) {
	int lPos = temp[1] - '0';
	int rPos = temp[2] - '0';
	lPos--;
	rPos--;
	return lPos * orderOfMatrix + rPos;
}

void findLandUMatrix(KeyValuePair<std::string, float>& L, KeyValuePair<std::string, float>& U, const List<float>& A, bool isDoLittle) {
	int forUnitMatrix = 1;
	const int jump = (int)sqrt(L.size);
	int rowCount = 1;
	std::string key;
	int temp = 0;

	//computing L
	for (int i = 0; i < L.size; i++) {
		if (i % jump == 0 && i > 0) {
			rowCount++;
			forUnitMatrix++;
			forUnitMatrix += jump;
		}

		if (isDoLittle) {
			if (i == forUnitMatrix - 1) { //put 1 in the diagonals
				key = "1";
			}
			else if (i < forUnitMatrix) {
				temp = (i % jump) + 1;
				key = "L" + std::to_string(rowCount) + std::to_string(temp);
			}
			else {
				key = "0";
			}
		}
		else {
			if (i < forUnitMatrix) {
				temp = (i % jump) + 1;
				key = "L" + std::to_string(rowCount) + std::to_string(temp);
			}
			else {
				key = "0";
			}
		}

		L.key[i] = key;
	}

	int forUpperTri = 0;
	rowCount = 1;

	//computing U
	for (int i = 0; i < U.size; i++) {
		if (i % jump == 0 && i > 0) {
			rowCount++;
			forUpperTri++;
			forUpperTri += jump;
		}

		if (isDoLittle) {
			if (i < forUpperTri) {
				key = "0";
			}
			else {
				temp = (i % jump) + 1;
				key = "U" + std::to_string(rowCount) + std::to_string(temp);
			}
		}
		else {
			if (i == forUpperTri) {	//puting 1 in the diagonals
				key = "1";
			}
			else if (i < forUpperTri) {
				key = "0";
			}
			else {
				temp = (i % jump) + 1;
				key = "U" + std::to_string(rowCount) + std::to_string(temp);
			}
		}

		U.key[i] = key;
	}

	KeyValuePair<std::string, float> LxU(L.size);
	int indexL = 0;
	int indexU = 0;
	std::string product = "";

	//computing L x U
	for (int i = 0; i < LxU.size; i += jump) {
		indexL = i;
		indexU = 0;

		for (int j = 0; j < LxU.size + 1; j++) {
			if (j % jump == 0 && j > 0) {
				indexL = i;
				indexU = indexU % jump + 1;

				if (product == "")
					LxU.key[i + j / jump - 1] = "0";
				else
					LxU.key[i + j / jump - 1] = product;

				product = "";
			}

			if (j == LxU.size)
				break;


			if (L.key[indexL] == "0" || U.key[indexU] == "0") {
				product = product + "";
			}
			else if (L.key[indexL] == "1") {
				if (product != "")
					product = product + "+" + (std::string)U.key[indexU];
				else
					product = U.key[indexU];
			}
			else if (U.key[indexU] == "1") {
				if (product != "")
					product = product + "+" + (std::string)L.key[indexL];
				else
					product = L.key[indexL];
			}
			else {
				if (product != "")
					product = product + "+" + (std::string)L.key[indexL] + "*" + (std::string)U.key[indexU];
				else
					product = (std::string)L.key[indexL] + "*" + (std::string)U.key[indexU];
			}

			indexL++;
			indexU += jump;
		}
	}
	
	out << "=========================L Matrix=========================\n";
	L.display(jump, 0);
	out << "\n=========================U Matrix=========================\n";
	U.display(jump, 0);
	out << "\n=========================LxU Matrix=========================\n";
	LxU.display(jump, 0);
	out << "\n=========================Now I am computing L and U by the formula LxU = A... =========================\n";

	std::string* checkPrev = new std::string[L.size]{};
	std::string tempStr;
	std::string newFound;
	std::string findReq = "";

	temp = 0;
	float valWithFound = 1.0f;
	float manipulateVal = 0.0f;
	int index = 0;
	bool isBreak = false;
	//computing L and U
	for (int i = 0; i < L.size; i++) {

		if (LxU.key[i].size() > 3) {
			while (true) {

				tempStr = LxU.key[i];
				temp = tempStr.find("+");
				if (temp == -1) {
					newFound = tempStr;
					isBreak = true;
				}
				else {
					LxU.key[i] = tempStr.substr(temp + 1);
					newFound = tempStr.substr(0, temp);
				}

				temp = newFound.find("*");
				if (temp == -1) { //length of the string is 3, we only need to do subtraction
					index = findIndex(newFound, jump);
					if (checkPrev[index] != "") { //this value is known and can be used for manipulation
						if (newFound[0] == 'L')
							manipulateVal += L.value[index];
						else
							manipulateVal += U.value[index];
					}
					else {	//check to find the unknown value
						checkPrev[index] = newFound;
						findReq = newFound;
					}
				}
				else {
					std::string str = newFound.substr(0, temp);
					newFound = newFound.substr(temp + 1);
					index = findIndex(str, jump);
					temp = findIndex(newFound, jump);
					if (findReq != "") { //already found the unknown value, no need to go in the else part, simply multiply the values
						manipulateVal = manipulateVal + (L.value[index] * U.value[temp]);
					}
					else {
						if (checkPrev[index] != "" && checkPrev[temp] != "") { //known values found simply multiply them
							manipulateVal = manipulateVal + (L.value[index] * U.value[temp]);
						}
						else if (checkPrev[index] == "") { //check finds the unknown value
							checkPrev[index] = str;
							valWithFound = U.value[temp];
							findReq = str;
						}
						else {	//check finds the unknown value
							checkPrev[temp] = newFound;
							valWithFound = L.value[index];
							findReq = newFound;
						}
					}


				}

				if (isBreak) { //all the the info is known lets break the loop
					index = findIndex(findReq, jump);

					if (findReq[0] == 'L') {
						L.value[index] = ((float)A.list[index] - manipulateVal) / valWithFound;
						L.value[index] = round(L.value[index] * 1000) / 1000;
					}
					else {
						U.value[index] = ((float)A.list[index] - manipulateVal) / valWithFound;
						U.value[index] = round(U.value[index] * 1000) / 1000;
					}
					break;
				}

			}
			findReq = "";
			valWithFound = 1.0f;
			manipulateVal = 0.0f;
			isBreak = false;
		}
		else {
			tempStr = LxU.key[i];
			index = findIndex(tempStr, jump);

			if (tempStr[0] == 'L') {
				L.value[index] = (float)A.list[index];
				L.value[index] = round(L.value[index] * 1000) / 1000;
			}
			else {
				U.value[index] = (float)A.list[index];
				U.value[index] = round(U.value[index] * 1000) / 1000;
			}
			checkPrev[index] = tempStr;
		}
	}

	if (isDoLittle) {
		for (int i = 0; i < L.size; i++) {
			if (L.key[i] == "1") {
				L.value[i] = 1;
			}
		}
	}
	else {
		for (int i = 0; i < L.size; i++) {
			if (U.key[i] == "1") {
				U.value[i] = 1;
			}
		}
	}
	out << "\n=========================L Matrix=========================\n";
	L.display(jump, 1);
	out << "\n=========================U Matrix=========================\n";
	U.display(jump, 1);
}

void computeResults(const KeyValuePair<std::string, float>& L, const KeyValuePair<std::string, float>& Y, const List<float>& B, const char isXorY) {

	const int jump = B.size;
	int indexL = 0;

	KeyValuePair<std::string, float> LxY(jump);

	// Now computing L x Y  OR U x X
	std::ostringstream strStream;
	std::string product = "";
	int index = 0;
	for (int i = 0; i < L.size; i += jump) {
		indexL = i;

		for (int j = 0; j < LxY.size + 1; j++) {
			if (j % jump == 0 && j > 0) {
				indexL = i;

				LxY.key[index] = product;

				//std::cout << product << std::endl;
				product = "";
				index++;
			}

			if (j == LxY.size)
				break;


			if (L.value[indexL] == 0) {
				product = product + "";
			}
			else if (L.value[indexL] == 1) {
				if (product != "")
					product = product + "+" + (std::string)Y.key[j];
				else
					product = Y.key[j];
			}
			else {
				strStream << L.value[indexL];
				if (product != "") {
					product = product + "+" + strStream.str() + "*" + (std::string)Y.key[j];
				}
				else {
					product = strStream.str() + "*" + (std::string)Y.key[j];
				}
				strStream.str("");
				strStream.clear();

			}

			indexL++;
		}
	}

	if (isXorY == 'Y') {
		out << "=========================LxY Matrix=========================\n\n";
	}
	else {
		out << "=========================UxX Matrix=========================\n\n";
	}
	LxY.display(1, 0);

	// computing values of Y form LxY = B OR computing X from UxX=Y
	std::string tempStr = "";
	std::string findReq = "";
	std::string valWithFind = "";
	int operatorPos = 0;
	float result = 0.0f;
	index = 0;

	bool isX = false;
	int temp;
	if (isXorY == 'X')
		isX = true;
	for (int i = 0; i < LxY.size; i++) {
		if (isX) {
			temp = i;
			i = (jump - 1) - i;
		}
		operatorPos = LxY.key[i].find("+");
		if (operatorPos == -1) {
			operatorPos = LxY.key[i].find("*");
			if (operatorPos == -1) {
				Y.value[i] = B.list[i];
			}
			else {
				tempStr = LxY.key[i].substr(0, operatorPos);
				Y.value[i] = B.list[i] / std::stof(tempStr);
			}
			Y.value[i] = round(Y.value[i] * 1000) / 1000;
		}
		else {
			while (true) {
				operatorPos = LxY.key[i].find("+");
				if (operatorPos == -1) {
					tempStr = LxY.key[i];
					operatorPos = tempStr.find(isXorY);
					index = tempStr[operatorPos + 1] - '0';

					operatorPos = LxY.key[i].find("*");
					if (findReq != "") {
						if (operatorPos == -1) {
							result += Y.value[index - 1];
						}
						else {
							tempStr = tempStr.substr(0, operatorPos);
							result = result + (std::stof(tempStr) * Y.value[index - 1]);
						}
					}
					else {
						if (operatorPos == -1) {
							findReq = tempStr;
						}
						else {
							valWithFind = tempStr.substr(0, operatorPos);
							findReq = tempStr.substr(operatorPos + 1);
						}
					}
					break;
				}

				tempStr = LxY.key[i].substr(0, operatorPos);
				LxY.key[i] = LxY.key[i].substr(operatorPos + 1);

				operatorPos = tempStr.find(isXorY);
				index = tempStr[operatorPos + 1] - '0';
				if (index == i + 1) {
					operatorPos = tempStr.find("*");
					if (operatorPos == -1) {
						findReq = tempStr;
					}
					else {
						valWithFind = tempStr.substr(0, operatorPos);
						findReq = tempStr.substr(operatorPos + 1);
					}
				}
				else {
					operatorPos = tempStr.find("*");
					if (operatorPos == -1) {
						result += Y.value[index - 1];
					}
					else {
						tempStr = tempStr.substr(0, operatorPos);
						result = result + (std::stof(tempStr) * Y.value[index - 1]);

					}
				}

			}

			if (valWithFind == "")
				Y.value[i] = B.list[i] - result;
			else
				Y.value[i] = (B.list[i] - result) / std::stof(valWithFind);

			Y.value[i] = round(Y.value[i] * 1000) / 1000;

			result = 0.0f;
			valWithFind = "";
			findReq = "";
		}
		if (isX)
			i = temp;
	}

	//Y is actually X or Y
	if (isXorY == 'X') {
		out << "\n=========================Finally compute X with by the formula U x X = Y...=========================\n\n";
		out << "=========================X Matrix=========================\n\n";
	}
	else {
		out << "\n=========================Now Computing Y by the formula LxY = B...=========================\n\n";
		out << "=========================Y Matrix=========================\n\n";
	}
	Y.display(1, 2);
}

void putDataInInputFile(List<float>& A, List<float>& B, int orderSize) {
	try
	{
		out.open(inputFile);
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to open input data file\n";
	}

	out << orderSize << "\n\n";
	for (int i = 0; i < orderSize; i++) {
		for (int j = i*orderSize; j < orderSize*(i+1); j++) {
			out << A.list[j] << "   ";
		}
		out << "\n";
	}
	out << "\n";

	for (int i = 0; i < orderSize; i++) {
		out << B.list[i] << "\n";
	}

	out.close();

}

// Matrix A-> 8 4 -5 2 -5 1 3 1 8
// Matrix B-> 38 32 28

// Matrix A-> 2 1 3 4 3 10 2 4 17 
// Matrix B-> 11 28 31

void optionsForDataReading() {
	std::cout << "1) Read data from file\n";
	std::cout << "2) Input data here\n";
}

void options() {
	std::cout << "\n1) 2x2 matrix\n";
	std::cout << "2) 3x3 matrix\n";
	std::cout << "3) 4x4 matrix\n";
	std::cout << "4) 5x5 matrix\n";
}

void forFileWriting(int order) {
	try
	{
		out.open(outputFile);
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to open data file\n";
		return;
	}
	out << "==================================================================\n";
	out << "=========================LU Decomposition=========================\n";
	out << "==================================================================\n\n";
	out << "=========================Matrix is Choosen of the order: " << order << " x " << order << "=========================\n\n";
	out.close();

}

int main() {

	int orderOfMatrix = 0;
	List<float> A;
	List<float> B;
	int elementsInMatrix;
	int chooseOption;
	std::cout << "\t\tLU Decomposition\n";
	optionsForDataReading();
	std::cout << "Choose an option: ";
	std::cin >> chooseOption;
	if (chooseOption != 1 && chooseOption != 2) {
		std::cout << "\nInvalid option selected\n";
		return 0;
	}
	else if (chooseOption == 1) {
		std::string temp = "";
		try
		{
			in.open(inputFile);
		}
		catch (const std::exception&)
		{
			std::cout << "Failed to open input file\n";
			return 0;
		}
		
		std::getline(in, temp);
		orderOfMatrix = std::stoi(temp);
		elementsInMatrix = orderOfMatrix * orderOfMatrix;
		A.initializeList(elementsInMatrix);
		B.initializeList(orderOfMatrix);

		for (int i = 0; i < elementsInMatrix; i++) {
			in >> temp;
			A.list[i] = (float) std::stof(temp);
		}

		for (int i = 0; i < orderOfMatrix; i++) {
			in >> temp;
			B.list[i] = (float) std::stof(temp);
		}
		in.close();

	}
	else if (chooseOption == 2) {
		
		options();
		std::cout << "Choose the order of the matrix: ";
		std::cin >> orderOfMatrix;

		if (orderOfMatrix < 1 || orderOfMatrix > 4) {
			std::cout << "Invalid option selected\n";
			return 0;
		}
		orderOfMatrix++;
		elementsInMatrix = orderOfMatrix * orderOfMatrix;

		A.initializeList(elementsInMatrix);
		B.initializeList(orderOfMatrix);
		takeInput(A, B);

		putDataInInputFile(A, B, orderOfMatrix);
	}

		
	forFileWriting(orderOfMatrix);

	//1 2 3 4 5 6
	

	KeyValuePair<std::string, float> L(elementsInMatrix);
	KeyValuePair<std::string, float> U(elementsInMatrix);

	int selectMethod = 0;
	std::cout << "1) Do Little's Method\n";
	std::cout << "2) Crout's Method\n";
	std::cout << "Choose method: ";
	std::cin >> selectMethod;

	if (selectMethod < 1 || selectMethod > 2) {
		std::cout << "Invalid Method selected\n";
		return 0;
	}
	bool isDoLittle = true; //here sets do littles method 
	if (selectMethod == 2)
		isDoLittle = false; //here sets crout's method
	
	try
	{
		out.open(outputFile, std::ios::app);
	}
	catch (const std::exception&)
	{

	}
	if (isDoLittle) {
		out << "=========================We are computing AX = B by using Do little's Method=========================\n\n";
	}
	else {
		out << "=========================We are computing AX = B by using Crout's Method=========================\n\n";
	}

	out << "\n=========================A Matrix=========================\n";
	A.display(orderOfMatrix);

	out << "\n=========================B Matrix=========================\n";
	B.display(1);
	out << "\n=========================X Matrix=========================\n";
	KeyValuePair<std::string, float> X(orderOfMatrix);

	for (int i = 0; i < orderOfMatrix; i++) {
		X.key[i] = "X" + std::to_string(i + 1);
	}

	X.display(1, 0);
	out << "\n=========================Solution=========================\n";
	out << "\n==========================================================\n\n";
	findLandUMatrix(L, U, A, isDoLittle);

	


	KeyValuePair<std::string, float> Y(orderOfMatrix);

	for (int i = 0; i < orderOfMatrix; i++) {
		Y.key[i] = "Y" + std::to_string(i + 1);
	}

	computeResults(L, Y, B, 'Y');

	
	List<float> newY(orderOfMatrix);
	for (int i = 0; i < orderOfMatrix; i++) {
		newY.list[i] = Y.value[i];
	}
	computeResults(U, X, newY, 'X');

	out.close();
	std::cout << "\n\n================================See the file for results================================\n";
	return 0;
}


