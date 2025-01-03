#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Book {
public:
    string title;
    string author;

    Book(const string& title, const string& author)
        : title(title), author(author) {
    }
};

class BookManager {
private:
    vector<Book> books;

public:
    // 책 추가
    void addBook(const string& title, const string& author) {
        books.emplace_back(title, author);
        cout << "책이 추가되었습니다: " << title << " by " << author << endl;
    }

    // 책 검색 (제목으로)
    Book* findBookByTitle(const string& title) {
        auto it = find_if(books.begin(), books.end(),
            [&title](const Book& book) { return book.title == title; });
        return (it != books.end()) ? &(*it) : nullptr;
    }

    // 책 검색 (저자로)
    vector<Book*> findBooksByAuthor(const string& author) {
        vector<Book*> foundBooks;
        for (auto& book : books) {
            if (book.author == author) {
                foundBooks.push_back(&book);
            }
        }
        return foundBooks;
    }

    // 모든 책 출력
    void displayAllBooks() const {
        if (books.empty()) {
            cout << "현재 등록된 책이 없습니다." << endl;
            return;
        }

        cout << "현재 도서 목록:" << endl;
        for (const auto& book : books) {
            cout << "- " << book.title << " by " << book.author << endl;
        }
    }

    vector<Book>& getBooks() { return books; }  // non-const로 수정
};

class BorrowManager {
private:
    struct BorrowRecord {
        string title;
        string author;
        int stock;

        BorrowRecord(const string& title, const string& author, int stock)
            : title(title), author(author), stock(stock) {
        }
    };

    vector<BorrowRecord> borrowRecords;
    const int maxStock = 3; // 최대 재고 수 (향후 변경 가능)
    BookManager& bookManager;  // BookManager 참조 추가

public:
    BorrowManager(BookManager& bookManager) : bookManager(bookManager) {  // BookManager 참조로 받음
        for (auto& book : bookManager.getBooks()) {
            borrowRecords.emplace_back(book.title, book.author, maxStock); // 초기 재고 3권, 저자 추가
        }
    }

    void addBooks() {
        for (auto& book : bookManager.getBooks()) {
            auto it = find_if(borrowRecords.begin(), borrowRecords.end(),
                [&book](const BorrowRecord& record) { return record.title == book.title; });

            if (it == borrowRecords.end()) {
                borrowRecords.emplace_back(book.title, book.author, maxStock); // 새로운 책 추가
            }
        }
    }

    void rentBook(const string& title) {
        auto it = find_if(borrowRecords.begin(), borrowRecords.end(),
            [&title](const BorrowRecord& record) { return record.title == title; });

        if (it == borrowRecords.end()) {
            cout << "책을 찾을 수 없습니다: " << title << endl;
            return;
        }

        if (it->stock > 0) {
            --it->stock;
            cout << "대출되었습니다: " << title << " by " << it->author << " (남은 재고: " << it->stock << ")" << endl;
        }
        else {
            cout << "대출 불가: " << title << " (재고 없음)" << endl;
        }
    }

    void rentBooksByAuthor(const string& author) {
        auto books = bookManager.findBooksByAuthor(author);  // BookManager에서 책을 찾음
        if (books.empty()) {
            cout << "저자 " << author << "의 책을 찾을 수 없습니다." << endl;
            return;
        }

        bool allRented = true;
        for (auto& book : books) {
            auto it = find_if(borrowRecords.begin(), borrowRecords.end(),
                [&book](const BorrowRecord& record) { return record.title == book->title; });
            if (it != borrowRecords.end() && it->stock > 0) {
                rentBook(book->title);
            }
            else {
                allRented = false;
                cout << "대출 불가: " << book->title << " by " << book->author << " (재고 없음)" << endl;
            }
        }

        if (allRented) {
            cout << "모든 책이 대출되었습니다." << endl;
        }
    }

    void returnBook(const string& title) {
        auto it = find_if(borrowRecords.begin(), borrowRecords.end(),
            [&title](const BorrowRecord& record) { return record.title == title; });

        if (it == borrowRecords.end()) {
            cout << "책을 찾을 수 없습니다: " << title << endl;
            return;
        }

        if (it->stock < maxStock) {
            ++it->stock;
            cout << "반납되었습니다. 책이름: " << title << " 저자: " << it->author << " (남은 재고는 " << it->stock << "개 입니다.)" << endl;
        }
        else {
            cout << "반납 불가합니다 책이름:  " << title << " 저자: " << it->author << " (이미 재고가 가득 찼습니다.)" << endl;
        }
    }

    void returnBooksByAuthor(const string& author) {
        auto books = bookManager.findBooksByAuthor(author);  // BookManager에서 책을 찾음
        if (books.empty()) {
            cout << "저자 " << author << "의 책을 찾을 수 없습니다." << endl;
            return;
        }

        for (auto& book : books) {
            returnBook(book->title);
        }
    }

    void displayBorrowStatus() const {
        if (borrowRecords.empty()) {
            cout << "대여 가능한 책이 없습니다." << endl;
            return;
        }

        cout << "대여 상태 목록:" << endl;
        for (const auto& record : borrowRecords) {
            cout << "- " << record.title << " by " << record.author << " (재고: " << record.stock << ")" << endl;
        }
    }
};

int main() {
    BookManager bookManager;
    BorrowManager borrowManager(bookManager);  // BookManager 참조로 전달

    while (true) {
        cout << "\n도서관 관리 프로그램" << endl;
        cout << "1. 책 추가" << endl;
        cout << "2. 모든 책 출력" << endl;
        cout << "3. 대여 상태 보기" << endl;
        cout << "4. 책 대출 (제목으로)" << endl;
        cout << "5. 책 대출 (저자명으로)" << endl;
        cout << "6. 책 반납 (제목으로)" << endl;
        cout << "7. 책 반납 (저자명으로)" << endl;
        cout << "8. 종료" << endl;
        cout << "선택: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string title, author;
            cout << "책 제목: ";
            cin.ignore();
            getline(cin, title);
            cout << "책 저자: ";
            getline(cin, author);
            bookManager.addBook(title, author);
            borrowManager.addBooks();  // 책 추가 후 대출 기록 업데이트
        }
        else if (choice == 2) {
            bookManager.displayAllBooks();
        }
        else if (choice == 3) {
            borrowManager.displayBorrowStatus();
        }
        else if (choice == 4) {
            string title;
            cout << "대출할 책 제목: ";
            cin.ignore();
            getline(cin, title);
            borrowManager.rentBook(title);
        }
        else if (choice == 5) {
            string author;
            cout << "대출할 책 저자: ";
            cin.ignore();
            getline(cin, author);
            borrowManager.rentBooksByAuthor(author);
        }
        else if (choice == 6) {
            string title;
            cout << "반납할 책 제목: ";
            cin.ignore();
            getline(cin, title);
            borrowManager.returnBook(title);
        }
        else if (choice == 7) {
            string author;
            cout << "반납할 책 저자: ";
            cin.ignore();
            getline(cin, author);
            borrowManager.returnBooksByAuthor(author);
        }
        else if (choice == 8) {
            cout << "프로그램을 종료합니다." << endl;
            break;
        }
        else {
            cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
        }
    }

    return 0;
}
