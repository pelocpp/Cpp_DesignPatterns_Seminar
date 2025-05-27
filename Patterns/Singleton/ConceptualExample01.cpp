// ===========================================================================
// ConceptualExample01.cpp
// ===========================================================================

#include <iostream>
#include <thread>
#include <mutex>


// DATEI.cpp  : Globale Variable : Die die Klasse 'Singleton' implementiert.

// Alle Clients verwenden ein HEader-File bzgl. Klasse 'Singleton',
// um MEthode Singleton::getInstance aufrufen zu können.

// Damit haben ALLE Clients einen Pointer: Singleton*

// Multi Threading: ==>

namespace ConceptualExample01 {

    class Singleton final
    {
    private:
        static Singleton* m_instance;
        static std::mutex m_mutex;

    private:
        Singleton() = default;

        // jegliches Kopieren / Verschieben unterbinden
        Singleton(const Singleton&) = delete;
        Singleton(Singleton&&) noexcept = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton& operator=(Singleton&&) noexcept = delete;

    public:
        static Singleton* getInstance()    //  Single Threading Kontext
        {
            if (m_instance == nullptr) {           // Lazy-Ansatz
                m_instance = new Singleton{};
            }

            return m_instance;
        }

        static Singleton* getInstanceThreadSafe()
        {
            {
                std::lock_guard<std::mutex> lock{ m_mutex };   //  lock  // 

                if (m_instance == nullptr) {
                    m_instance = new Singleton{};
                }
            }                                                 // unlock

            return m_instance;
        }

        static Singleton* getInstanceThreadSafe_Cpp17()
        {
            if (std::lock_guard<std::mutex> lock{ m_mutex }; m_instance == nullptr) {
                m_instance = new Singleton{};
            }

            return m_instance;
        }

        void doThis() const
        {
            std::cout << "doing this on singleton instance (" << this << ")." << std::endl;
        }

        void doThat() const
        {
            std::cout << "doing that on singleton instance (" << this << ")." << std::endl;
        }
    };

    Singleton* Singleton::m_instance{ nullptr };
    std::mutex Singleton::m_mutex{};
}

void test_singleton_01()
{
    using namespace ConceptualExample01;

    Singleton::getInstance()->doThis();
    Singleton::getInstance()->doThat();
}

void test_singleton_02()
{
    using namespace ConceptualExample01;

    auto singleton1 { Singleton::getInstance() };
    auto singleton2 { Singleton::getInstance() };

    std::cout << std::boolalpha << (singleton1 == singleton2) << std::endl;

    auto singleton3 { Singleton::getInstanceThreadSafe() };
    auto singleton4 { Singleton::getInstanceThreadSafe() };

    std::cout << std::boolalpha << (singleton3 == singleton4) << std::endl;
}

// ===========================================================================
// End-of-File
// ===========================================================================
