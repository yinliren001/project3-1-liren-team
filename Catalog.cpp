//****************************************************************************************
//
//	INCLUDE FILES
//
//****************************************************************************************
#include	<fstream>
#include	<iostream>
#include	<string>
#include	"Catalog.h"

using namespace std;

bool	Catalog::AddCategory(uint64_t categoryNumber, const string& name)
{
    //If catagory number exists, return false
    if (this->myCatalog.count(categoryNumber) != 0) return false;
    //Update info in both catalog name and catalog detail info maps
    else {
        this->myCatalog.insert(std::pair<uint64_t, map<uint64_t, string>>(categoryNumber, *new map<uint64_t, string>()));
        this->catalogNames.insert(std::pair<uint64_t, string>(categoryNumber, name));
        this->totalProductsInCatalog.insert(std::pair<uint64_t, uint64_t>(categoryNumber, 0));
        this->catalogCount++;
        return true;
    }
}

bool	Catalog::AddProduct(uint64_t categoryNumber, uint64_t productNumber, const string& name)
{
    //if catagory does NOT exist, return false
    if (this->myCatalog.count(categoryNumber) == 0) return false;
    else {
        //if both product and catagory exist, return false
        if (this->myCatalog.at(categoryNumber).count(productNumber) != 0) return false;
        else {
            this->myCatalog.at(categoryNumber).insert(std::pair<uint64_t, string>(productNumber, name));
            //update product count under this catalog
            map<uint64_t, uint64_t>::iterator pos = totalProductsInCatalog.find(categoryNumber);
            pos->second++;
            return true;
        }
    }
}

uint64_t	Catalog::GetCategoryCount()
{
    return this->catalogCount;
}

int64_t	Catalog::GetProductCount(uint64_t categoryNumber)
{
    //If there's no such catagory, return -1 as invalid return value
    if (this->myCatalog.count(categoryNumber) == 0) return -1;
    else {
        map<uint64_t, uint64_t>::const_iterator pos = totalProductsInCatalog.find(categoryNumber);
        return pos->second;
    }
}

bool	Catalog::Load(const string& fileName)
{
    uint64_t catalogNum_Global;
	ifstream myfile(fileName);
    std::cout << "Loading from [" << fileName << "]" << std::endl;
    system("pwd");
    std::ifstream ifs(fileName.c_str());
    if (!ifs.is_open() || !ifs.good()) {
        return false;
    }
     while (!ifs.eof()) {
         string lineBuffer;
         getline(ifs, lineBuffer);
         if (lineBuffer.size() == 0) continue;
         if (lineBuffer.find("Category") != string::npos) {
             //get catalog number
             uint64_t catalogNum;
             string catalogNumInString;
             string catalogName;
             int catalog_Num_start = lineBuffer.find_first_not_of ('\t', 8);
             int catalog_Num_end = lineBuffer.find_first_of('\t', catalog_Num_start);
             int pos = catalog_Num_start;
             while (pos != catalog_Num_end) {
                 catalogNumInString += lineBuffer.at(pos++);
             }
             catalogNum = std::stoull(catalogNumInString.c_str());
             catalogNum_Global = catalogNum;
             //get catalog name
             int catalog_Name_start = lineBuffer.find_first_not_of('\t', catalog_Num_end);
             int catalog_Name_end = lineBuffer.length();
             pos = catalog_Name_start;
             while (pos != catalog_Name_end) {
                 catalogName += lineBuffer.at(pos++);
             }
             this->AddCategory(catalogNum, catalogName);
         }
         else {
             uint64_t productNum;
             string productNumInString;
             string productName;
             //Get product number
             int productNumStart = 0;
             int productNumEnd = lineBuffer.find_first_of('\t', productNumStart);
             int productNumPos = productNumStart;
             while (productNumPos != productNumEnd) {
                 productNumInString += lineBuffer.at(productNumPos++);
             }
             productNum = std::stoull(productNumInString.c_str());
             
             //Get Product Name
             int productNameStart = lineBuffer.find_first_not_of('\t', productNumEnd);
             int productNameEnd = lineBuffer.length();
             int productNamePos = productNameStart;
             while (productNamePos != productNameEnd) {
                 productName += lineBuffer.at(productNamePos++);
             }
             this->AddProduct(catalogNum_Global, productNum, productName);
         }
     }
    return true;
}

//****************************************************************************************
//
//	Show... methods
//
//	The stream argument allows thess functions to write its output to any output stream,
//	such as cout or a file, that the caller passes. The argument's name "stream" should
//	be used just as you would use "cout":
//
//		stream << "X = " << x << endl;
//
//	The output will go where the caller intended without special action by this function.
//
//****************************************************************************************

bool	Catalog::ShowProduct(ostream& stream, uint64_t categoryNumber, uint64_t productNumber)
{
    if (this->myCatalog.count(categoryNumber) == 0) return false;
    else {
        if (this->myCatalog.at(categoryNumber).find(productNumber) == this->myCatalog.at(categoryNumber).end()) return false;
        else {
            map<uint64_t, string>::const_iterator pos = myCatalog.at(categoryNumber).find(productNumber);
            stream << pos->first << "\t" << pos->second;
            return true;
        }
    }
}

bool	Catalog::ShowCategory(ostream& stream, uint64_t categoryNumber)
{
    //if the input catagory does not exist, return false
    if (this->myCatalog.count(categoryNumber) == 0) return false;
    else {
        map<uint64_t, string>::iterator catagoryNameIterator = this->catalogNames.find(categoryNumber);
        map<uint64_t, string>::iterator productIterator = this->myCatalog.at(categoryNumber).begin();
        stream << "Category\t" << catagoryNameIterator->first << "\t" << catagoryNameIterator->second << endl;
        while (productIterator != this->myCatalog.at(categoryNumber).end()) {
            stream << productIterator->first << "\t" << productIterator->second << endl;
            productIterator++;
        }
        return true;
    }
}

bool	Catalog::ShowAll(ostream& stream)
{
    // If there's no catalog in the instance, return false
    if (this->catalogCount == 0) return false;
    map<uint64_t, string>::iterator catagoryNameIterator = this->catalogNames.begin();
    while (catagoryNameIterator != this->catalogNames.end()) {
        this->ShowCategory(stream, catagoryNameIterator->first);
        catagoryNameIterator++;
    }
	return true;
}

