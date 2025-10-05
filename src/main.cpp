#include "Greeter.h"
#include "RecipeDatabase.h" // main에서도 알아야 함
#include <iostream>

int main() {
    // 1. 데이터베이스 객체를 먼저 생성
    RecipeDatabase db;

    // 2. 파일에서 데이터를 불러와서 db를 채움
    string filename = "Recipe.txt";
    if (db.loadFromFile(filename)) {
        cout << "Successfully loaded recipes from " << filename << endl;
    } else {
        cout << "Could not find " << filename << ". Starting with an empty database." << endl;
    }

    // 3. 데이터가 채워진 db를 Greeter에게 전달하며 Greeter 생성
    Greeter greeter(db);

    // 4. Greeter의 메인 루프 실행
    greeter.run();

    return 0;
}