#include "md5.h"
#include "md5.cpp"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream> 
#include <time.h>
using namespace std;
 
 
int main()
{
	
	string myurl= "http://api.fanyi.baidu.com/api/trans/vip/translate?";
    string appid = "20190309000275477";    //replace myAppid with your own appid
    string q = "apple";          //replace apple with your own text to be translate, ensure that the input text is encoded with UTF-8!
    string from = "auto";          //replace en with your own language type of input text
    string to = "zh";            //replace zh with your own language type of output text
    string secret_key = "Z59hbYXTrSxh9VHCYecR";   //replace mySecretKey with your own mySecretKey
    
    string salt;
    srand(time(NULL));
    stringstream itos;
    itos<<rand();itos>>salt;
    itos.str("");
    //����һ���������Ϊsalt 
	
	string sign=appid+q+salt+secret_key;
	MD5 md5;
	md5.update(sign);
	sign=md5.toString();
	md5.reset();
	
	string BDTApiUrl=myurl+"q="+q+"&from="+from+"&to="+to+"&appid="+appid+"&salt="+salt+"&sign="+sign;
	cout<<BDTApiUrl;    
    
    
	/*MD5 md5;                 //����MD5����  
	md5.update("123456"); //��Ϊupdate����ֻ����string���ͣ�����ʹ��getbuffer()����ת��CStringΪstring  
	cout << md5.toString();
	md5.reset();//����
	md5.update("rancho");
	cout <<endl<< md5.toString();*/
	return 0;
}
