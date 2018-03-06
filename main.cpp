#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include "logins.h"
#include <string>

// Для корректной работы freeaddrinfo в MinGW
// Подробнее: http://stackoverflow.com/a/20306451
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <lm.h>

// Необходимо, чтобы линковка происходила с DLL-библиотекой
// Для работы с сокетам
#pragma comment(lib, "Ws2_32.lib")

using std::cerr;
using std::ifstream;
using std::map;
using std::string;

int main()
{
    WSADATA wsaData; // служебная структура для хранение информации
    // о реализации Windows Sockets
    // старт использования библиотеки сокетов процессом
    // (подгружается Ws2_32.dll)
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Если произошла ошибка подгрузки библиотеки
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << "\n";
        return result;
    }

    struct addrinfo* addr = NULL; // структура, хранящая информацию
    // об IP-адресе  слущающего сокета

    // Шаблон для инициализации структуры адреса
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET; // AF_INET определяет, что будет
    // использоваться сеть для работы с сокетом
    hints.ai_socktype = SOCK_STREAM; // Задаем потоковый тип сокета
    hints.ai_protocol = IPPROTO_TCP; // Используем протокол TCP
    hints.ai_flags = AI_PASSIVE; // Сокет будет биндиться на адрес,
    // чтобы принимать входящие соединения

    // Инициализируем структуру, хранящую адрес сокета - addr
    // Наш HTTP-сервер будет висеть на 8000-м порту локалхоста
    result = getaddrinfo("127.0.0.1", "8000", &hints, &addr);

    // Если инициализация структуры адреса завершилась с ошибкой,
    // выведем сообщением об этом и завершим выполнение программы
    if (result != 0) {
        cerr << "getaddrinfo failed: " << result << "\n";
        WSACleanup(); // выгрузка библиотеки Ws2_32.dll
        return 1;
    }

    // Создание сокета
    int listen_socket = socket(addr->ai_family, addr->ai_socktype,
                               addr->ai_protocol);
    // Если создание сокета завершилось с ошибкой, выводим сообщение,
    // освобождаем память, выделенную под структуру addr,
    // выгружаем dll-библиотеку и закрываем программу
    if (listen_socket == INVALID_SOCKET) {
        cerr << "Error at socket: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr);
        WSACleanup();
        return 1;
    }

    // Привязываем сокет к IP-адресу
    result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

    // Если привязать адрес к сокету не удалось, то выводим сообщение
    // об ошибке, освобождаем память, выделенную под структуру addr.
    // и закрываем открытый сокет.
    // Выгружаем DLL-библиотеку из памяти и закрываем программу.
    if (result == SOCKET_ERROR) {
        cerr << "bind failed with error: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr);
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    // Инициализируем слушающий сокет
    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "listen failed with error: " << WSAGetLastError() << "\n";
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }


    const int max_client_buffer_size = 1024;
    char buf[max_client_buffer_size], buf2[max_client_buffer_size], buf3[max_client_buffer_size], exp[5];
    char root[]="C:\\Users\\Emerael\\Desktop\\untitled2\\";//корень сервера, укажите ручками чо уж там
    int client_socket = INVALID_SOCKET;
    //мап вида расширение - mime-тип
    map<string, string> content_type={{"css", "text/css"},{"html", "text/html"}, {"jpg", "image/jpeg"}, {"jpeg", "image/jpeg"},
                                      {"png", "image/png"}, {"js", "application/javascript"}};

    for (;;) {
        // Принимаем входящие соединения
        client_socket = accept(listen_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            cerr << "accept failed: " << WSAGetLastError() << "\n";
            closesocket(listen_socket);
            WSACleanup();
            return 1;
        }

        result = recv(client_socket, buf, max_client_buffer_size, 0);

        std::stringstream response; // сюда будет записываться ответ клиенту
        std::stringstream response_body; // тело ответа

        if (result == SOCKET_ERROR) {
            // ошибка получения данных
            cerr << "recv failed: " << result << "\n";
            closesocket(client_socket);
        } else if (result == 0) {
            // соединение закрыто клиентом
            cerr << "connection closed...\n";
        } else if (result > 0) {
            // Мы знаем фактический размер полученных данных, поэтому ставим метку конца строки
            // В буфере запроса.
            buf2[0]='\0';
            buf3[0]='\0';
            exp[0]='\0';
            buf[result] = '\0';
            cerr<<buf;
            strcpy(buf2, strchr(buf, '/')+1);
            *strchr(buf2, ' ') = '\0';
            ifstream page;
            std::string mess;
            if(strstr(buf2, "log")==buf2&&strstr(buf2,"login")!=buf2){
                strcpy(buf3,buf2+10);//buf3 есть Username&pass=pwd
                strcpy(buf2,strchr(buf3,'=')+1);
                *strchr(buf3,'&')='\0';
                bool fl = false;
                if(data.find(buf3)==data.end()){//Нет логина в "базе"
                    mess = "User does not exist.";
                }
                else
                    if(data.find(buf3)->second!=buf2) { //wrong pass
                        mess = "Wrong password";
                    }
                    else{
                        fl=true;
                        mess = "Welcome!";
                    }
                buf2[0]='\0';
                strcat(buf2, root);
                strcat(buf2, "users.html");
                page.open(buf2, std::ios_base::in);
                response_body << "<title>Ololoshki</title>"
                              << "<link rel='stylesheet' type='text/css' href='../style.css'>"
                              << "<script src='../jquery.js' type='text/javascript'></script>"
                              << "<script>username='Guest';usertitle='Stranger';ava='/img/noava.png';group=0;</script>"
                              << page.rdbuf()
                              << "<Script>$('#content').html('"<<mess<<"');</script>";
                strcat(exp,"html");
                response << "HTTP/1.1 200 OK\r\n"
                         << "Version: HTTP/1.1\r\n";
                if(fl)
                    response << "Set-cookie: username=" << buf3 << "\r\n";
                response << "Content-Type: " << content_type[exp] << " charset=utf-8\r\n"
                         << "Content-Length: " << response_body.str().length();
                response << "\r\n\r\n" << response_body.str();
            }
            if(strchr(buf2,'.')){//запрашивается файл
                if(strchr(buf2, '?')){
                    *strchr(buf2, '?') = '\0';
                }
                strcat(exp,strchr(buf2,'.')+1);//вытаскиваем его расширение чтобы потом корректно отдать mime
                while(strchr(buf2,'/')){//меняем слеши из html на обратные
                    *strchr(buf2,'/')='\\';
                }
                strcat(buf3,root);//цепляем имя корня
                strcat(buf3,buf2);//цепляем название файла
                page.open(buf3, std::ios::binary);//открываем файл как бинарник
            } else if(strstr(buf2, "profile")){//страница чьего-то профиля
                strcat(buf3, root);//закинули корень
                strcat(buf3, "profile.html");
                page.open(buf3);
                if(!data.count(strchr(buf2,'/')+1)){
                    response_body<<"<title>OOOPS!</title>"
                                 << "<script src='../jquery.js' type='text/javascript' defer=''></script>"
                                 << "<script src=\"../nf.js\" type=\"text/javascript\" defer=''></script>";
                }
                response_body << "<title>Ololoshki - "<< strchr(buf2,'/')+1<< "</title>"
                              << "<link rel='stylesheet' type='text/css' href='../style.css'>"
                              << "<script src='../jquery.js' type='text/javascript' defer=''></script>"
                              << "<script>username='Guest';usertitle='Stranger';ava='/img/noava.png';group=0;</script>"
                              << "<script src='/users/" <<strchr(buf2,'/')+1<<".js' type='text/javascript' defer=''></script>"
                              << "<script src=\"../scripts.js\" type=\"text/javascript\" defer=''></script>"
                              << "<script src=\"../upscr.js\" type=\"text/javascript\" defer=''></script>";
            } else if(strstr(buf2, "users")) {//список профилей офицеров
                std::stringstream users;
                for(auto it=data.begin(); it!=data.end(); ++it){
                    users << "<a href=\"../profile/" << it->first << "\">" << it->first << "</a><br>";
                }
                response_body << "<title>Ololoshki - Officers list</title>"
                              << "<link rel='stylesheet' type='text/css' href='../style.css'>"
                              << "<script src='../jquery.js' type='text/javascript'></script>"
                              << "<script>username='Guest';usertitle='Stranger';ava='/img/noava.png';group=0;</script>"
                              << "<script src=\"../scripts.js\" type=\"text/javascript\" defer=''></script>";
                strcat(exp,"txt");//расширение - текстовая
                strcat(buf2,".html");//дописываем .html чтобы забрать файл
                strcat(buf3,root);//цепляем корень
                strcat(buf3,buf2);//цепляем имя файла
                page.open(buf3, std::ios_base::in);
                // корректно обработалось
                // кидаем содержимое файла
                response_body << page.rdbuf()<< "<script>$('#content').html('"<<users.rdbuf()<<"');$('#content').attr('class', 'ut');</script>";
                // Формируем весь ответ вместе с заголовками
                response << "HTTP/1.1 200 OK\r\n"
                         << "Version: HTTP/1.1\r\n"
                         << "Content-Type: " << content_type[exp] << " charset=utf-8\r\n"
                         << "Content-Length: " << response_body.str().length()
                         << "\r\n\r\n"
                         << response_body.str();
                // Отправляем ответ клиенту с помощью функции send
                result = send(client_socket, response.str().c_str(),
                              response.str().length(), 0);

                if (result == SOCKET_ERROR) {
                    // произошла ошибка при отправле данных
                    cerr << "send failed: " << WSAGetLastError() << "\n";
                }
                // Закрываем соединение к клиентом
                closesocket(client_socket);
                continue;
            } else if(buf2[0]!='\0'){//непустое название страницы
                response_body << "<title>Ololoshki - "<< buf2<< "</title>"
                              << "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">"
                              << "<script src=\"jquery.js\" type=\"text/javascript\" defer=\"\"></script>"
                              << "<script src=\"scripts.js\" type=\"text/javascript\" defer=\"\"></script>"
                              << "<script>username='Guest';usertitle='Stranger';ava='/img/noava.png';group=0;</script>";
                strcat(exp,"txt");//расширение - текстовая
                strcat(buf2,".html");//дописываем .html чтобы забрать файл
                strcat(buf3,root);//цепляем корень
                strcat(buf3,buf2);//цепляем имя файла
                page.open(buf3, std::ios_base::in);
            }else {//главная
                strcat(exp,"txt");
                response_body << "<title>Ololoshki</title>"
                              << "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">"
                              << "<script src=\"jquery.js\" type=\"text/javascript\" defer=\"\"></script>"
                              << "<script src=\"scripts.js\" type=\"text/javascript\" defer=\"\"></script>"
                              << "<script>username='Guest';usertitle='Stranger';ava='/img/noava.png';group=0;</script>";
                page.open("C:\\Users\\Emerael\\Desktop\\untitled2\\index.html");
                std::string s;
            }
            if(!page.is_open()){//файл не нашло, сюда можно засунуть кастомную 404
                response << "HTTP/1.1 404 Not Found\r\n"
                         << "Version: HTTP/1.1\r\n";
            } else {
                // корректно обработалось
                // кидаем содержимое файла
                response_body << page.rdbuf();
                // Формируем весь ответ вместе с заголовками
                response << "HTTP/1.1 200 OK\r\n"
                         << "Version: HTTP/1.1\r\n"
                         << "Content-Type: " << content_type[exp] << " charset=utf-8\r\n"
                         << "Content-Length: " << response_body.str().length()
                         << "\r\n\r\n"
                         << response_body.str();
            }
            // Отправляем ответ клиенту с помощью функции send
            result = send(client_socket, response.str().c_str(),
                          response.str().length(), 0);

            if (result == SOCKET_ERROR) {
                // произошла ошибка при отправле данных
                cerr << "send failed: " << WSAGetLastError() << "\n";
            }
            // Закрываем соединение к клиентом
            closesocket(client_socket);
        }
    }

    // Убираем за собой
    closesocket(listen_socket);
    freeaddrinfo(addr);
    WSACleanup();
    return 0;
}
