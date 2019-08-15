#include "json/json.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

#include <cstdlib>
#include <cstring>
#include <functional>
#include <list>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#define MAX_FILE_LENGTH 20000

using namespace std;

class WriterMemoryClass
{
public:
	// Helper Class for reading result from remote host
	WriterMemoryClass()
	{
		this->m_pBuffer = NULL;
		this->m_pBuffer = (char*) malloc(MAX_FILE_LENGTH * sizeof(char));
		this->m_Size = 0;
	};

	~WriterMemoryClass()
	{
		if (this->m_pBuffer)
			free(this->m_pBuffer);
	};

	void* Realloc(void* ptr, size_t size)
	{
		if(ptr)
			return realloc(ptr, size);
		else
			return malloc(size);
	};

	// Callback must be declared static, otherwise it won't link...
	size_t WriteMemoryCallback(char* ptr, size_t size, size_t nmemb)
	{
		// Calculate the real size of the incoming buffer
		size_t realsize = size * nmemb;

		// (Re)Allocate memory for the buffer
		m_pBuffer = (char*) Realloc(m_pBuffer, m_Size + realsize);

		// Test if Buffer is initialized correctly & copy memory
		if (m_pBuffer == NULL) {
			realsize = 0;
		}

		memcpy(&(m_pBuffer[m_Size]), ptr, realsize);
		m_Size += realsize;


		// return the real size of the buffer...
		return realsize;
	};


	void print() 
	{
		std::cout << "Size: " << m_Size << std::endl;
		std::cout << "Content: " << std::endl << m_pBuffer << std::endl;
	}

	// Public member vars
	char* m_pBuffer;
	size_t m_Size;
};


int main(int argc, char *argv[])
{

	char *url = "http://120.76.101.183:8084/park/car/in";

	try
	{
		curlpp::Cleanup cleaner;
		curlpp::Easy request;

		WriterMemoryClass mWriterChunk;

		// Set the writer callback to enable cURL 
		// to write result in a memory area
		using namespace std::placeholders;
		curlpp::types::WriteFunctionFunctor functor = std::bind(&WriterMemoryClass::WriteMemoryCallback, &mWriterChunk, _1, _2, _3);

		curlpp::options::WriteFunction *test = new curlpp::options::WriteFunction(functor);
		request.setOpt(test);

		// Setting the URL to retrive.
		request.setOpt(new curlpp::options::Url(url));
		request.setOpt(new curlpp::options::Verbose(true));
        std::list<std::string> header; 
        header.push_back("Content-Type: application/json;charset=UTF-8"); 
    
        request.setOpt(new curlpp::options::HttpHeader(header)); 

         Json::Value item;
        item["parkInId"]=Json::Value("123123");
        item["parkCode"]=Json::Value("10001");
        item["carNumber"]=Json::Value("闽A1234");
        item["carType"]=Json::Value("x");
        item["inTime"]=Json::Value("2019-08-06 16:19:04");
        std::string jsonout = item.toStyledString();

        request.setOpt(new curlpp::options::PostFields(jsonout.c_str()));
        request.setOpt(new curlpp::options::PostFieldSize(jsonout.size()));

		request.perform();

		mWriterChunk.print();
	}
	
	catch ( curlpp::LogicError & e )
	{
		std::cout << e.what() << std::endl;
	}
	
	catch ( curlpp::RuntimeError & e )
	{
		std::cout << e.what() << std::endl;
	}
}
