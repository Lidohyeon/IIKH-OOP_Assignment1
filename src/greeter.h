#ifndef GREETER_H
#define GREETER_H

#include <iostream>
#include <limits>
#include "RecipeDatabase.h" // 데이터베이스 클래스를 사용하기 위해 포함

using namespace std;

class Greeter {
private:
    RecipeDatabase& db; // 데이터베이스 객체에 대한 '참조'

    //===== Private Helper Functions (UI 처리) =====

    void showWelcome() {
        cout << "\n";
        cout << "██╗██╗██╗██╗  ██╗██╗  ██╗" << endl;
        cout << "██║██║██║██║ ██╔╝██║  ██║" << endl;
        cout << "██║██║██║█████╔╝ ███████║" << endl;
        cout << "██║██║██║██╔═██╗ ██╔══██║" << endl;
        cout << "██║██║██║██║  ██╗██║  ██║" << endl;
        cout << "╚═╝╚═╝╚═╝╚═╝  ╚═╝╚═╝  ╚═╝" << endl;
        cout << "\nWelcome to IIKH Data Management System!" << endl;
        cout << "=========================================" << endl;
    }

    void showMenu() {
        cout << "\n===== Main Menu =====\n";
        cout << "1. Insert New Recipe\n";
        cout << "2. Search for a Recipe\n";
        cout << "3. Delete a Recipe\n";
        cout << "4. Edit a Recipe\n";
        cout << "5. Sort Recipes by Title\n";
        cout << "6. List All Recipes\n";
        cout << "7. Save and Exit\n";
        cout << "==========================\n";
        cout << "> ";
    }

    void handleInsert() {
        db.insertRecipe(); // 데이터베이스 객체에게 삽입을 요청
    }

    void handleSearch() {
        db.searchRecipe(); // 데이터베이스 객체에게 검색을 요청
    }

    void handleDelete() {
        db.deleteRecipe(); // 데이터베이스 객체에게 삭제를 요청
    }

    void handleEdit() {
        db.editRecipe(); // 데이터베이스 객체에게 수정을 요청
    }
    
    void handleSort() {
        db.sortRecipe(); // 데이터베이스 객체에게 정렬을 요청
        cout << "Recipes have been sorted." << endl;
        db.displayAll(); // 정렬된 결과를 보여줌
    }

public:
    // 생성자: 사용할 데이터베이스를 외부에서 받음
    Greeter(RecipeDatabase& database) : db(database) {}

    // 메인 루프를 실행하는 함수
    void run() {
        showWelcome();
        int choice = 0;
        while (choice != 7) {
            showMenu();
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = 0; // 잘못된 입력 시 선택 초기화
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 엔터 키 버퍼 비우기

            switch (choice) {
                case 1: handleInsert(); break;
                case 2: handleSearch(); break;
                case 3: handleDelete(); break;
                case 4: handleEdit(); break;
                case 5: handleSort(); break;
                case 6: db.displayAll(); break;
                case 7:
                    db.saveToFile();
                    cout << "Changes saved. Exiting the program. Thank you!" << endl;
                    break;
                default:
                    cout << "Invalid selection. Please try again." << endl;
                    break;
            }
        }
    }
};

#endif // GREETER_H