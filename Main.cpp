#include "json/json.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <curl/curl.h>

using namespace std;

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) 
{
    string data((const char*) ptr, (size_t) size * nmemb);

    *((stringstream*) stream) << data << endl;

    return size * nmemb;
}

int main()
{
    CURL *curl;
    CURLcode res;
    char tmp_str[256] = { 0 };
    std::stringstream out;

    //HTTP报文头  
    struct curl_slist* headers = NULL;

    char *url = "http://120.76.101.183:8084/park/car/in";

    curl = curl_easy_init();

    if(curl)
    {
        //构建json
        
        Json::Value item;
        item["parkInId"]=Json::Value("123123");
        item["parkCode"]=Json::Value("10001");
        item["carNumber"]=Json::Value("闽A1234");
        item["carType"]=Json::Value("x");
        item["inTime"]=Json::Value("2019-08-06 16:19:04");
        std::string jsonout = item.toStyledString();
        /*
        jsonout = AsciiToUtf8(jsonout);
        
				string jsonout="";
        */

        //设置url
        curl_easy_setopt(curl, CURLOPT_URL, url);

        //设置http发送的内容类型为JSON
        //构建HTTP报文头  
        sprintf(tmp_str, "Content-Length: %s", jsonout.c_str());
        headers=curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
        //headers=curl_slist_append(headers, tmp_str);//在请求头中设置长度,请求会失败,还没找到原因

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        //curl_easy_setopt(curl,  CURLOPT_CUSTOMREQUEST, "POST");//自定义请求方式
        curl_easy_setopt(curl, CURLOPT_POST, 1);//设置为非0表示本次操作为POST

        // 设置要POST的JSON数据
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonout.size());//设置上传json串长度,这个设置可以忽略

        // 设置接收数据的处理函数和存放变量
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//设置回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);//设置写数据
        res = curl_easy_perform(curl);//执行
        curl_slist_free_all(headers); /* free the list again */
        string str_json = out.str();//返回请求值 
        printf("%s",str_json.c_str()); 

       /* always cleanup */ 
      curl_easy_cleanup(curl);
     } 

     return 0;
 }