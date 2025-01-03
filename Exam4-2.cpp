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
    // å �߰�
    void addBook(const string& title, const string& author) {
        books.emplace_back(title, author);
        cout << "å�� �߰��Ǿ����ϴ�: " << title << " by " << author << endl;
    }

    // å �˻� (��������)
    Book* findBookByTitle(const string& title) {
        auto it = find_if(books.begin(), books.end(),
            [&title](const Book& book) { return book.title == title; });
        return (it != books.end()) ? &(*it) : nullptr;
    }

    // å �˻� (���ڷ�)
    vector<Book*> findBooksByAuthor(const string& author) {
        vector<Book*> foundBooks;
        for (auto& book : books) {
            if (book.author == author) {
                foundBooks.push_back(&book);
            }
        }
        return foundBooks;
    }

    // ��� å ���
    void displayAllBooks() const {
        if (books.empty()) {
            cout << "���� ��ϵ� å�� �����ϴ�." << endl;
            return;
        }

        cout << "���� ���� ���:" << endl;
        for (const auto& book : books) {
            cout << "- " << book.title << " by " << book.author << endl;
        }
    }

    vector<Book>& getBooks() { return books; }  // non-const�� ����
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
    const int maxStock = 3; // �ִ� ��� �� (���� ���� ����)
    BookManager& bookManager;  // BookManager ���� �߰�

public:
    BorrowManager(BookManager& bookManager) : bookManager(bookManager) {  // BookManager ������ ����
        for (auto& book : bookManager.getBooks()) {
            borrowRecords.emplace_back(book.title, book.author, maxStock); // �ʱ� ��� 3��, ���� �߰�
        }
    }

    void addBooks() {
        for (auto& book : bookManager.getBooks()) {
            auto it = find_if(borrowRecords.begin(), borrowRecords.end(),
                [&book](const BorrowRecord& record) { return record.title == book.title; });

            if (it == borrowRecords.end()) {
                borrowRecords.emplace_back(book.title, book.author, maxStock); // ���ο� å �߰�
            }
        }
    }

    void rentBook(const string& title) {
        auto it = find_if(borrowRecords.begin(), borrowRecords.end(),
            [&title](const BorrowRecord& record) { return record.title == title; });

        if (it == borrowRecords.end()) {
            cout << "å�� ã�� �� �����ϴ�: " << title << endl;
            return;
        }

        if (it->stock > 0) {
            --it->stock;
            cout << "����Ǿ����ϴ�: " << title << " by " << it->author << " (���� ���: " << it->stock << ")" << endl;
        }
        else {
            cout << "���� �Ұ�: " << title << " (��� ����)" << endl;
        }
    }

    void rentBooksByAuthor(const string& author) {
        auto books = bookManager.findBooksByAuthor(author);  // BookManager���� å�� ã��
        if (books.empty()) {
            cout << "���� " << author << "�� å�� ã�� �� �����ϴ�." << endl;
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
                cout << "���� �Ұ�: " << book->title << " by " << book->author << " (��� ����)" << endl;
            }
        }

        if (allRented) {
            cout << "��� å�� ����Ǿ����ϴ�." << endl;
        }
    }

    void returnBook(const string& title) {
        auto it = find_if(borrowRecords.begin(), borrowRecords.end(),
            [&title](const BorrowRecord& record) { return record.title == title; });

        if (it == borrowRecords.end()) {
            cout << "å�� ã�� �� �����ϴ�: " << title << endl;
            return;
        }

        if (it->stock < maxStock) {
            ++it->stock;
            cout << "�ݳ��Ǿ����ϴ�. å�̸�: " << title << " ����: " << it->author << " (���� ���� " << it->stock << "�� �Դϴ�.)" << endl;
        }
        else {
            cout << "�ݳ� �Ұ��մϴ� å�̸�:  " << title << " ����: " << it->author << " (�̹� ��� ���� á���ϴ�.)" << endl;
        }
    }

    void returnBooksByAuthor(const string& author) {
        auto books = bookManager.findBooksByAuthor(author);  // BookManager���� å�� ã��
        if (books.empty()) {
            cout << "���� " << author << "�� å�� ã�� �� �����ϴ�." << endl;
            return;
        }

        for (auto& book : books) {
            returnBook(book->title);
        }
    }

    void displayBorrowStatus() const {
        if (borrowRecords.empty()) {
            cout << "�뿩 ������ å�� �����ϴ�." << endl;
            return;
        }

        cout << "�뿩 ���� ���:" << endl;
        for (const auto& record : borrowRecords) {
            cout << "- " << record.title << " by " << record.author << " (���: " << record.stock << ")" << endl;
        }
    }
};

int main() {
    BookManager bookManager;
    BorrowManager borrowManager(bookManager);  // BookManager ������ ����

    while (true) {
        cout << "\n������ ���� ���α׷�" << endl;
        cout << "1. å �߰�" << endl;
        cout << "2. ��� å ���" << endl;
        cout << "3. �뿩 ���� ����" << endl;
        cout << "4. å ���� (��������)" << endl;
        cout << "5. å ���� (���ڸ�����)" << endl;
        cout << "6. å �ݳ� (��������)" << endl;
        cout << "7. å �ݳ� (���ڸ�����)" << endl;
        cout << "8. ����" << endl;
        cout << "����: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string title, author;
            cout << "å ����: ";
            cin.ignore();
            getline(cin, title);
            cout << "å ����: ";
            getline(cin, author);
            bookManager.addBook(title, author);
            borrowManager.addBooks();  // å �߰� �� ���� ��� ������Ʈ
        }
        else if (choice == 2) {
            bookManager.displayAllBooks();
        }
        else if (choice == 3) {
            borrowManager.displayBorrowStatus();
        }
        else if (choice == 4) {
            string title;
            cout << "������ å ����: ";
            cin.ignore();
            getline(cin, title);
            borrowManager.rentBook(title);
        }
        else if (choice == 5) {
            string author;
            cout << "������ å ����: ";
            cin.ignore();
            getline(cin, author);
            borrowManager.rentBooksByAuthor(author);
        }
        else if (choice == 6) {
            string title;
            cout << "�ݳ��� å ����: ";
            cin.ignore();
            getline(cin, title);
            borrowManager.returnBook(title);
        }
        else if (choice == 7) {
            string author;
            cout << "�ݳ��� å ����: ";
            cin.ignore();
            getline(cin, author);
            borrowManager.returnBooksByAuthor(author);
        }
        else if (choice == 8) {
            cout << "���α׷��� �����մϴ�." << endl;
            break;
        }
        else {
            cout << "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���." << endl;
        }
    }

    return 0;
}
