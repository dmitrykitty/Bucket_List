#include <iostream>
#include <numeric> // std::accumulate
#include <string>
#include <cmath>
#include <algorithm> // std::sort
#include <gtest/gtest.h>

#if __has_include("../SortedUniqueVectoredList.h")
    #include "../SortedUniqueVectoredList.h"
#elif __has_include("SortedUniqueVectoredList.h")
    #include "SortedUniqueVectoredList.h"
#else
    #error "File 'SortedUniqueVectoredList.h' not found!"
#endif

namespace
{
using namespace std;
using namespace ::testing;

constexpr size_t MAX_TEXT_LENGTH = 750;

string multiplyText(const string& text2Multiply, size_t multiplyWith)
{
    string multipliedText;
    multipliedText.reserve(text2Multiply.size() * multiplyWith);
    for (size_t i=0; i < multiplyWith; ++i)
    {
        multipliedText += text2Multiply;
    }
    return multipliedText;
}
} // namespace



class SortedUniqueVectoredListTests : public ::testing::Test
{
    SortedUniqueVectoredList sortedUniqueVectoredList;

public:
    template<typename T>
    unsigned CeilOfDivision(T divident, T divisor)
    {
        double dividentAsDouble = divident;
        double divisorAsDouble = divisor;

        return static_cast<unsigned>( ceil(dividentAsDouble / divisorAsDouble) );
    }
};

TEST_F(SortedUniqueVectoredListTests, defaultConstructor_sizeOfEmptyContainerIsZero)
{
    const SortedUniqueVectoredList sortedUniqueVectoredList;
    ASSERT_EQ(0, sortedUniqueVectoredList.size());
    ASSERT_EQ(0, sortedUniqueVectoredList.capacity());
    ASSERT_EQ(0, sortedUniqueVectoredList.bucket_count());
}

TEST_F(SortedUniqueVectoredListTests, pushBack_sizeOfContainerIncreaseAfterCallingPushBack)
{
    // source: Sw. Jan Pawel II
    constexpr const char texts[][MAX_TEXT_LENGTH] =
    {
        "Musicie od siebie wymagac, nawet gdyby inni od was nie wymagali",
        "Najbardziej tworcza ze wszystkich prac jest praca nad soba, ktora pozwala odnajdywac urok mlodosci. Nie ma wiekszego bogactwa w narodzie nad swiatlych obywateli",
        "Czlowiek nie jest w stanie pracowac, gdy nie widzi sensu pracy, gdy sens ten przestaje byc dla niego przejrzysty, gdy zostaje mu niejako przesloniety. Praca ludzka stoi w posrodku calego zycia spolecznego. Poprzez nia ksztaltuje sie sprawiedliwosc i milosc spoleczna, jezeli cala dziedzina pracy rzadzi wlasciwy lad moralny",
        "Kazdy z was, mlodzi przyjaciele, znajduje tez w zyciu jakies swoje Westerplatte. Jakis wymiar zadan, ktore musi podjac i wypelnic. Jakas sluszna sprawe, o ktora nie mozna nie walczyc. Jakis obowiazek, powinnosc, od ktorej nie mozna sie uchylic. Nie mozna zdezerterowac",
        "Przyszlosc zaczyna sie dzisiaj, nie jutro",
    };
    constexpr size_t expectedSize = sizeof(texts) / sizeof(texts[0]);
    constexpr size_t expectedBucketCount = 1;

    SortedUniqueVectoredList sortedUniqueVectoredList;
    for (auto text : texts)
    {
        sortedUniqueVectoredList.insert(text);
    }
    ASSERT_EQ(expectedSize, sortedUniqueVectoredList.size());
    ASSERT_TRUE(sortedUniqueVectoredList.capacity() > 0);
    ASSERT_EQ(expectedBucketCount, sortedUniqueVectoredList.bucket_count());
}

TEST_F(SortedUniqueVectoredListTests, arrayIndexingOperator_testsInTheSamePositionsAsPushed)
{
    // source: https://8segment.pl/polskie-przyslowia.php (polskie przyslowia):
    constexpr const char sortedTexts[][MAX_TEXT_LENGTH] =
    {
        "Bez pracy nie ma kolaczy",
        "Bog pomaga temu, ktory sam rozwiazuje wlasne problemy",
        "Gdyby kozka nie skakala, to by nozki nie zlamala",
        "Jak sobie poscielisz, tak sie wyspisz",
        "Prosta droga najkrotsza"
    };
    constexpr size_t expectedSize = sizeof(sortedTexts) / sizeof(sortedTexts[0]);
    size_t sizeCounter = 1;

    SortedUniqueVectoredList sortedUniqueVectoredList;
    for (auto text : sortedTexts)
    {
        sortedUniqueVectoredList.insert(text);
        ASSERT_EQ(sizeCounter, sortedUniqueVectoredList.size()) << "last added text: " << text;
        ++sizeCounter;
    }

    ASSERT_EQ(expectedSize, sortedUniqueVectoredList.size());
    for (size_t i=0; i < expectedSize; ++i)
    {
        ASSERT_EQ(sortedTexts[i], sortedUniqueVectoredList[i]) << "index: " << i;
    }
}

TEST_F(SortedUniqueVectoredListTests, arrayIndexingOperator_throwingExceptionWhenOutOfRange)
{
    // source: http://cytatybaza.pl/cytat/jest-tylko-jedno-dobro-wiedza-i-jedno-zlo.html (Sokrates):
    constexpr const char text[] = "Jest tylko jedno dobro: wiedza, i jedno zlo: ignorancja";
    constexpr size_t expectedSize = 1;

    SortedUniqueVectoredList sortedUniqueVectoredList;

    ASSERT_THROW(sortedUniqueVectoredList[0], std::out_of_range);

    sortedUniqueVectoredList.insert(text);
    ASSERT_EQ(expectedSize, sortedUniqueVectoredList.size());
    ASSERT_EQ(text, sortedUniqueVectoredList[0]);

    ASSERT_THROW(sortedUniqueVectoredList[expectedSize], std::out_of_range);
}

TEST_F(SortedUniqueVectoredListTests, copyConstructor_textsDeeplyCopied)
{
    // source: https://pl.wikiquote.org/wiki/Terry_Pratchett
    constexpr const char sortedTexts[][MAX_TEXT_LENGTH] =
    {
        "A czym byliby ludzie bez milosci?\nGINACYM GATUNKIEM, odparl Smierc",
        "Cisza jest tylko brakiem halasu",
        "Gdyby porazka nie znala kary, sukces nie bylby nagroda",
        "Historia ma zwyczaj zmieniac ludzi, ktorzy mysla, ze ja zmieniaja",
        "Jesli nienormalne trwa dostatecznie dlugo, staje sie normalnym",
    };
    constexpr size_t expectedSize = sizeof(sortedTexts) / sizeof(sortedTexts[0]);

    SortedUniqueVectoredList sortedUniqueVectoredList;
    for (auto text : sortedTexts)
    {
        sortedUniqueVectoredList.insert(text);
    }

    const SortedUniqueVectoredList sortedUniqueVectoredListCopy(sortedUniqueVectoredList);
    ASSERT_EQ(expectedSize, sortedUniqueVectoredListCopy.size());
    ASSERT_EQ(expectedSize, sortedUniqueVectoredList.size());

    for (size_t i=0; i < expectedSize; ++i)
    {
        ASSERT_EQ(sortedTexts[i], sortedUniqueVectoredListCopy[i]) << "index: " << i;
        ASSERT_EQ(sortedTexts[i], sortedUniqueVectoredList[i]) << "index: " << i;
    }

    const SortedUniqueVectoredList ptrCStringVectorCopy2(sortedUniqueVectoredListCopy);
    for (size_t i=0; i < expectedSize; ++i)
    {
        ASSERT_EQ(sortedTexts[i], ptrCStringVectorCopy2[i]) << "index: " << i;
        ASSERT_EQ(sortedTexts[i], sortedUniqueVectoredListCopy[i]) << "index: " << i;
    }
}

TEST_F(SortedUniqueVectoredListTests, assignmentOperator_copyingContainerDeeply)
{
    // source: https://www.cytaty.info/cyprian-kamil-norwid.htm (Cyprian Norwid)
    constexpr const char sortedTexts[][MAX_TEXT_LENGTH] =
    {
        "Ojczyzna to wielki zbiorowy obowiazek.",
        "Oryginalnosc jest to sumiennosc w obliczu zrodel",
        "Polacy albowiem uzywaja powiedzenia nie ma co mowic wtedy, gdy jest bardzo wiele do mowienia",
        "Slowo jest ogien - milczenie jest lawa",
        "Z karafki napic sie mozna, uscisnawszy ja za szyjke i przechyliwszy ku ustom, ale kto ze zrodla pije, musi ukleknac i pochylic czolo",
    };
    constexpr size_t expectedSize = sizeof(sortedTexts) / sizeof(sortedTexts[0]);
    constexpr size_t expectedSizeOfEmptyContainer = 0;

    SortedUniqueVectoredList sortedUniqueVectoredList;
    for (auto text : sortedTexts)
    {
        sortedUniqueVectoredList.insert(text);
    }

    {
        SortedUniqueVectoredList sortedUniqueVectoredListCopy;
        ASSERT_EQ(expectedSizeOfEmptyContainer, sortedUniqueVectoredListCopy.size());

        sortedUniqueVectoredListCopy = sortedUniqueVectoredList;
        ASSERT_EQ(expectedSize, sortedUniqueVectoredListCopy.size());
        ASSERT_EQ(expectedSize, sortedUniqueVectoredList.size());

        for (size_t i=0; i < expectedSize; ++i)
        {
            ASSERT_EQ(sortedTexts[i], sortedUniqueVectoredListCopy[i]) << "index: " << i;
        }
    }
    for (size_t i=0; i < expectedSize; ++i)
    {
        ASSERT_EQ(sortedTexts[i], sortedUniqueVectoredList[i]) << "index: " << i;
    }
}

TEST_F(SortedUniqueVectoredListTests, assignmentOperator_assignmentCascadeAtOnceExpectingDeepCopyMultipleTimesEvenWithItselve)
{
    // source: https://quotepark.com/pl/autorzy/maksymilian-maria-kolbe/ (Sw. Maksymilian Maria Kolbe)
    constexpr const char sortedTexts[][MAX_TEXT_LENGTH] =
    {
        "Dobrze spelniac to co ode mnie zalezy, a dobrze znosic to, co ode mnie nie zalezy - oto jest cala doskonalosc i zrodlo prawdziwego szczescia na swiecie.",
        "Najwieksza ofiara - to ofiara z wlasnej woli",
        "Nie zmoga nas te cierpienia, tylko przetopia i zahartuja. Wielkich potrzeba ofiar naszych, aby okupic szczescie i pokojowe zycie tych, co po nas beda",
        "Wiedza nadyma i o tyle jest pozyteczna, o ile sluzy milosci",
        "Zadnej prawdy nikt nie moze zmienic - moze tylko szukac jej, znalezc ja i uznac, by do niej zycie dostosowac",
    };
    constexpr size_t expectedSize = sizeof(sortedTexts) / sizeof(sortedTexts[0]);

    SortedUniqueVectoredList sortedUniqueVectoredList, sortedUniqueVectoredListCopy1, sortedUniqueVectoredListCopy2;
    for (auto text : sortedTexts)
    {
        sortedUniqueVectoredList.insert(text);
    }

    sortedUniqueVectoredList = sortedUniqueVectoredListCopy1 = sortedUniqueVectoredListCopy2 = sortedUniqueVectoredList;

    ASSERT_EQ(expectedSize, sortedUniqueVectoredListCopy1.size());
    ASSERT_EQ(expectedSize, sortedUniqueVectoredListCopy2.size());

    for (size_t i=0; i < expectedSize; ++i)
    {
        ASSERT_EQ(sortedTexts[i], sortedUniqueVectoredListCopy1[i]) << "index: " << i;
        ASSERT_EQ(sortedTexts[i], sortedUniqueVectoredListCopy2[i]) << "index: " << i;
    }
}

TEST_F(SortedUniqueVectoredListTests, assignmentOperator_movingTextFromAnotherContainer)
{
    // source: http://www.wyszynski.psur.pl/cytaty.php
    constexpr const char sortedTexts[][MAX_TEXT_LENGTH] =
    {
        "(...) szczesliwy czlowiek, ktory jest z siebie niezadowolony, bo to jest punkt wyjscia",
        "Kazda milosc musi byc probowana i doswiadczana. Ale gdy wytrwa, doczeka sie nagrody - zwycieskiej radosci",
        "Nie trzeba ogladac sie na innych, na tych lub owych, moze na politykow, zadajac od nich, aby sie odmienili. Kazdy musi zaczac od siebie, abysmy prawdziwie sie odmienili",
        "Potrzeba w naszej Ojczyznie przykladu ofiary z siebie, aby czlowiek wspolczesny zapomnial o sobie, a myslal o drugich - o dobru rodziny, o wypelnieniu swego powolania i zadania zyciowego wobec innych, o dobru calego narodu.",
        R"(Tak czesto slyszymy zdanie: "Piekna i zaszczytna rzecza jest umrzec za Ojczyzne". Jednakze trudniej jest niekiedy zyc dla Ojczyzny. Mozna w odruchu bohaterskim oddac swoje zycie na polu walki, ale to trwa krotko. Wiekszym niekiedy bohaterstwem jest zyc, trwac, wytrzymac cale lata.)",
    };
    constexpr size_t expectedSize = sizeof(sortedTexts) / sizeof(sortedTexts[0]);
    constexpr size_t expectedSizeOfEmptyContainer = 0;

    SortedUniqueVectoredList sortedUniqueVectoredList;
    for (auto text : sortedTexts)
    {
        sortedUniqueVectoredList.insert(text);
    }

    SortedUniqueVectoredList sortedUniqueVectoredListCopy;
    ASSERT_EQ(expectedSize, sortedUniqueVectoredList.size());
    ASSERT_EQ(expectedSizeOfEmptyContainer, sortedUniqueVectoredListCopy.size());

    sortedUniqueVectoredListCopy = std::move(sortedUniqueVectoredList);
    ASSERT_EQ(expectedSizeOfEmptyContainer, sortedUniqueVectoredList.size());
    ASSERT_EQ(expectedSize, sortedUniqueVectoredListCopy.size());

    for (size_t i=0; i < expectedSize; ++i)
    {
        ASSERT_EQ(sortedTexts[i], sortedUniqueVectoredListCopy[i]) << "index: " << i;
    }

    ASSERT_THROW(sortedUniqueVectoredList[0], std::out_of_range);
}

TEST_F(SortedUniqueVectoredListTests, subtractOperator_expectReturningContainerContainingElementsFromFirstContainerWhichAreNotInAnotherContainers)
{
    // source: https://www.cytaty.info/arystoteles.htm, https://www.greelane.com/pl/humanistyka/filozofia/aristotle-quotes-117130/ (Arystoteles):
    constexpr const char texts1[][MAX_TEXT_LENGTH] =
    {
        "By zapragnac przyjazni nie potrzebujesz wiele czasu, lecz sama przyjazn jest owocem, ktory dojrzewa powoli.",
        "Inteligencja polega nie tylko na wiedzy, ale także umiejętności zastosowania wiedzy w praktyce",
        "Istnieją trzy rodzaje konstytucji: monarchia, arystokracja i oparta na własności, timokratyczna. Najlepsza jest  monarchia , najgorsza timokracja. Monarchia odchodzi od tyranii; król zwraca uwagę na interes swego ludu; tyran spogląda na swoje. Arystokracja przechodzi do oligarchii przez zło jej władców, którzy wbrew prawu rozdzielają to, co należy do miasta; większość dobrych rzeczy trafia do siebie i biura zawsze do tych samych ludzi, przywiązując największą wagę do bogactwa; tak więc władców jest niewielu i są złymi ludźmi, a nie najbardziej godnymi. Timokracja przechodzi w demokrację, ponieważ w obu rządzi większość.",
        "Nieświadomy człowiek wypowiada zdobytą wiedzę, mądry człowiek zadaje pytania i rozważa",
        "Prawdziwa sprawiedliwoscia jest przezyc to co sie uczynilo innym.",
        "Prawdziwa wiedza to znajomosc przyczyn.",
    };
    constexpr size_t expectedSize1 = sizeof(texts1) / sizeof(texts1[0]);

    // source: https://www.cytaty.info/sokrates.htm (Sokrates):
    constexpr const char texts2[][MAX_TEXT_LENGTH] =
    {
        "Jemy, aby zyc, nie zyjemy, aby jesc",
        "Coz komu z tego, ze zjadl wszystkie rozumy, jesli nie ma wlasnego",
        "Puste worki nadyma wiatr, bezmyslnych ludzi - proznosc",
    };
    constexpr size_t expectedSize2 = sizeof(texts2) / sizeof(texts2[0]);

    constexpr size_t expectedSize = expectedSize1 + expectedSize2;

    array<string, expectedSize> texts;
    std::copy(begin(texts1), end(texts1), begin(texts));
    std::copy(begin(texts2), end(texts2), begin(texts)+expectedSize1);
    std::sort(begin(texts), end(texts));


    SortedUniqueVectoredList sortedUniqueVectoredList1;
    for (const auto& text : texts)
    {
        sortedUniqueVectoredList1.insert(text);
    }

    SortedUniqueVectoredList sortedUniqueVectoredList2;
    for (auto text : texts2)
    {
        sortedUniqueVectoredList2.insert(text);
    }

    const SortedUniqueVectoredList addedVectors = sortedUniqueVectoredList1 - sortedUniqueVectoredList2;
    ASSERT_EQ(expectedSize1, addedVectors.size());

    for (size_t i=0; i < expectedSize1; ++i)
    {
        ASSERT_EQ(texts1[i], addedVectors[i]) << "index: " << i;
    }
}

TEST_F(SortedUniqueVectoredListTests, multiplyAssignmentOperator_expectedMultiplyingTextManyTimes)
{
    constexpr const char texts[][MAX_TEXT_LENGTH] =
    {
        "Anastazja",
        "Bonifia",
        "Cecylia"
    };
    constexpr size_t expectedTextsCount = sizeof(texts) / sizeof(texts[0]);

    SortedUniqueVectoredList sortedUniqueVectoredList;
    for (auto text : texts)
    {
        sortedUniqueVectoredList.insert(text);
    }

    const size_t multiplyWith = 4;
    sortedUniqueVectoredList *= multiplyWith;

    for (size_t i=0; i < expectedTextsCount; ++i)
    {
        const auto concatedTexts = multiplyText(texts[i], multiplyWith);
        ASSERT_EQ(concatedTexts, sortedUniqueVectoredList[i]) << "index: " << i;
    }
}

TEST_F(SortedUniqueVectoredListTests, pushBack_pushingNotSortedElements_expectedElementsInContainerSorted)
{
    // inspiration: https://www.eska.pl/news/najpopularniejsze-imiona-w-polsce-2020-mamy-nowy-numer-1-i-kilka-zaskoczen-lista-aa-Ln67-fhqv-xPLx.html
    constexpr const char notSortedTexts[][MAX_TEXT_LENGTH] =
    {
        "Antoni",
        "Jan",
        "Jakub",
        "Aleksander",
        "Franciszek",
        "Szymon",
        "Filip",
        "Mikołaj",
        "Stanisław",
        "Wojciech",
        "Zuzanna",
        "Julia",
    };
    constexpr size_t expectedTextsCount = sizeof(notSortedTexts) / sizeof(notSortedTexts[0]);

    array<string, expectedTextsCount> sortedTexts;
    std::copy(begin(notSortedTexts), end(notSortedTexts), begin(sortedTexts));
    sort(begin(sortedTexts), end(sortedTexts));

    SortedUniqueVectoredList sortedUniqueVectoredList;
    for (auto text : notSortedTexts)
    {
        sortedUniqueVectoredList.insert(text);
    }

    for (size_t i=0; i < expectedTextsCount; ++i)
    {
        ASSERT_EQ(sortedTexts[i], sortedUniqueVectoredList[i]) << "index: " << i;
    }
}

TEST_F(SortedUniqueVectoredListTests, pushBack_pushingNotSortedNotUniqueElements_expectedElementsInContainerUniqueAndSorted)
{
    constexpr const char namesInClass[][MAX_TEXT_LENGTH] =
    {
        "Karolina",
        "Ania",
        "Grzegorz",
        "Piotrek",
        "Bartek",
        "Klimek",
        "Krysia",
        "Zosia",
        "Karolina",
        "Piotrek",
        "Ania",
        "Karolina",
    };
    constexpr size_t peopleInClassCount = sizeof(namesInClass) / sizeof(namesInClass[0]);

    vector<string> sortedUniqueTexts;
    sortedUniqueTexts.reserve(peopleInClassCount);
    std::copy(begin(namesInClass), end(namesInClass), back_inserter(sortedUniqueTexts));
    sort(begin(sortedUniqueTexts), end(sortedUniqueTexts));
    sortedUniqueTexts.erase(std::unique(begin(sortedUniqueTexts), end(sortedUniqueTexts)), end(sortedUniqueTexts));

    SortedUniqueVectoredList sortedUniqueVectoredList;
    for (auto text : namesInClass)
    {
        sortedUniqueVectoredList.insert(text);
    }

    for (size_t i=0; i < sortedUniqueTexts.size(); ++i)
    {
        ASSERT_EQ(sortedUniqueTexts[i], sortedUniqueVectoredList[i]) << "index: " << i;
    }
}

TEST_F(SortedUniqueVectoredListTests, pushingManyElements_expectedAllOfThemInContainerAndNothingBadhappen)
{
    constexpr size_t elements2Add = 1000;

    SortedUniqueVectoredList sortedUniqueVectoredList;
    vector<string> elements;
    for (size_t i = 0; i < elements2Add; i++) {
        auto element2Add = "element_" + to_string(i);
        sortedUniqueVectoredList.insert(element2Add);
        elements.push_back(element2Add);
        ASSERT_EQ(elements.size(), sortedUniqueVectoredList.size());
    }

    sort(begin(elements), end(elements));
    for (size_t i=0; i < elements.size(); ++i)
    {
        ASSERT_EQ(elements[i], sortedUniqueVectoredList[i]) << "index: " << i;
    }
}

TEST_F(SortedUniqueVectoredListTests, conversion2StringOperator_expectedAllTextsConcatened)
{
    // inspiration: https://www.koty.pl/imiona-dla-kota/
    constexpr const char catsNames[][MAX_TEXT_LENGTH] =
    {
        "Mruczek",
        "Kropeczka",
        "Agresor",
        "Azorek",
        "Smerfetka",
        "Guantaramera",
        "Ciapek",
        "Latek"
        "Bunia",
        "Beza",
        "Newton",
    };
    constexpr size_t namesCount = sizeof(catsNames) / sizeof(catsNames[0]);

    vector<string> sortedCatsNames;
    sortedCatsNames.reserve(namesCount);
    std::copy(begin(catsNames), end(catsNames), back_inserter(sortedCatsNames));
    sort(begin(sortedCatsNames), end(sortedCatsNames));

    SortedUniqueVectoredList sortedUniqueVectoredList;
    for (auto text : catsNames)
    {
        sortedUniqueVectoredList.insert(text);
    }

    for (size_t i=0; i < sortedCatsNames.size(); ++i)
    {
        ASSERT_EQ(sortedCatsNames[i], sortedUniqueVectoredList[i]) << "index: " << i;
    }

    const auto expectedMergedCatsNames = std::accumulate(begin(sortedCatsNames), end(sortedCatsNames), string(""));

    const auto mergedCatsNames = static_cast<string>(sortedUniqueVectoredList);
    ASSERT_EQ(expectedMergedCatsNames, mergedCatsNames);
}

TEST_F(SortedUniqueVectoredListTests, copying_copyingEmptyContainer)
{
    SortedUniqueVectoredList sortedUniqueVectoredList;
    SortedUniqueVectoredList sortedUniqueVectoredListCopy = sortedUniqueVectoredList;

    ASSERT_EQ(0, sortedUniqueVectoredListCopy.size());
    ASSERT_EQ(0, sortedUniqueVectoredListCopy.size());

    sortedUniqueVectoredList = sortedUniqueVectoredListCopy;

    ASSERT_EQ(0, sortedUniqueVectoredListCopy.size());
    ASSERT_EQ(0, sortedUniqueVectoredListCopy.size());
}

#ifdef DOMJUDGE /// this is for online judge system
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    GTEST_FLAG(print_time) = false;
    return RUN_ALL_TESTS();
}
#endif // DOMJUDGE

