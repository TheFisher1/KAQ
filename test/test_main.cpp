//
// Created by Kaloyan Ribarov on 6.01.24.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/doctest.h"

#include <stdexcept>

#include "../src/royal_family_tree/exceptions/EmptyHierarchyException.h"
#include "../src/royal_family_tree/tree/Hierarchy.h"
#include "../src/royal_family_tree/exceptions/MemberNotFoundException.h"

std::vector<Heir> setUp() {
    std::vector<Heir> heirsTest;
    for (std::size_t i = 0; i < 50; ++i) {
        heirsTest.emplace_back("Pesho " + std::to_string(i), "07.07.1022" , Religion::PROTESTANTISM, true);
    }

    return heirsTest;
}

Hierarchy setHierarchy() {
    Hierarchy hierarchy;
    hierarchy.addMember({"James", "04.01.1231", Religion::PROTESTANTISM, true});
    hierarchy.addMember({"Charles", "05.02.1233", Religion::PROTESTANTISM, true}, "James");
    hierarchy.addMember({"William", "05.03.1234", Religion::PROTESTANTISM, true}, "James");
    hierarchy.addMember({"Elisabeth", "07.04.1233", Religion::PROTESTANTISM, false}, "James");

    hierarchy.addMember({"James II", "04.01.1245", Religion::PROTESTANTISM, true}, "Charles");
    hierarchy.addMember({"John", "04.02.1256", Religion::PROTESTANTISM, true}, "Charles");
    hierarchy.addMember({"Paul", "05.03.1258", Religion::PROTESTANTISM, true}, "Elisabeth");

    return hierarchy;
}

TEST_CASE("Copy constructor and operator=") {
    SUBCASE("CopyConstructorNoSharedMemory") {

        Hierarchy hierarchy = setHierarchy();

        Hierarchy h(hierarchy);

        CHECK(h.contains("James II"));

        h.addMember({"James V", "04.01.1245", Religion::PROTESTANTISM, true}, "Charles");
        CHECK(!hierarchy.contains("James V"));
    }

    SUBCASE("Operator=") {
        Hierarchy hierarchy = setHierarchy();

        Hierarchy h;

        CHECK(!h.contains("James II"));
        h = hierarchy;
        h.addMember({"James V", "04.01.1245", Religion::PROTESTANTISM, true}, "Charles");
        CHECK(!hierarchy.contains("James V"));
    }
}

TEST_CASE("IteratorIteratesCorrectly") {
    Hierarchy hierarchy;
    hierarchy.addMember({"James", "02.04.1066", Religion::PROTESTANTISM, true});
    hierarchy.addMember({"Williams", "3.03.1082", Religion::PROTESTANTISM, true}, "James");
    hierarchy.addMember({"John", "5.03.1083", Religion::PROTESTANTISM, true}, "James");

    std::vector<std::string> names = {"James", "Williams", "John"};
    std::size_t i = 0;

    for (Heir heir : hierarchy) {
        CHECK(heir.getName() == names[i++]);
    }

    hierarchy.addMember({"James II", "02.04.1084", Religion::PROTESTANTISM, true}, "Williams");
    hierarchy.addMember({"Charles III", "02.04.1066", Religion::PROTESTANTISM, true}, "John");

    names = {"James", "Williams", "James II", "John", "Charles III"};
    i = 0;
    for (Heir heir : hierarchy) {
        CHECK(heir.getName() == names[i++]);
    }

}

TEST_CASE("testAddMembers") {
    Hierarchy hierarchyTest;

    SUBCASE("testAddsMembersCorrectlyFirstElement") {
        hierarchyTest.addMember({"James I", "23.05.1566", Religion::PROTESTANTISM, true});

        CHECK(hierarchyTest.getMonarch() == Heir {"James I", "23.05.1566", Religion::PROTESTANTISM, true});
    }

    SUBCASE("testAddsMembersCorrectlyWhenThereAreMoreThanOne") {
        hierarchyTest.addMember({"Charles I", "24.07.1600",Religion::PROTESTANTISM, true});
        hierarchyTest.addMember({"James I", "22.05.1566",Religion::PROTESTANTISM, true}, "Charles I");
        hierarchyTest.addMember({"James II", "23.05.1566",Religion::PROTESTANTISM, true}, "Charles I");

        std::vector<std::string> names = {"Charles I", "James I", "James II"};
        std::size_t i = 0;
        for (Hierarchy::Iterator iter = hierarchyTest.begin(); iter != hierarchyTest.end(); ++iter) {
            CHECK(iter->getName() == names[i++]);
        }
    }

    SUBCASE("testAddMembersCatholicNewHeir") {
        hierarchyTest = setHierarchy();
        CHECK_THROWS_AS(hierarchyTest.addMember({"James IV", "22.12.1111", Religion::CATHOLICISM, true}), const std::invalid_argument&);
    }

    SUBCASE("testHierarchyThrowsExceptionWhenNoParentIsFound") {
        hierarchyTest.addMember({"Charles I", "24.07.1600",Religion::PROTESTANTISM, true});

        CHECK_THROWS_AS(hierarchyTest.addMember({"James III", "25.08.1611",Religion::PROTESTANTISM, true}, "Charles III"), const MemberNotFoundException&);
    }

    SUBCASE("testThrowsExceptionWhenEmptyParentName") {
        hierarchyTest.addMember({"James III", "25.08.1611",Religion::PROTESTANTISM, true});

        CHECK_THROWS_AS(hierarchyTest.addMember({"John III", "25.08.1611",Religion::PROTESTANTISM, true}), const std::invalid_argument&);
    }
}

TEST_CASE("Print Info") {
    Hierarchy hierarchy;
    hierarchy.addMember({"James I", "23.05.1566",Religion::PROTESTANTISM, true});

    hierarchy.addMember({"Charles I", "24.07.1600",Religion::PROTESTANTISM, true}, "James I");
    hierarchy.addMember({"Elizabeth Stuart", "11.08.1596",Religion::PROTESTANTISM, true}, "James I");

    hierarchy.addMember({"Charles II", "23.4.1630",Religion::PROTESTANTISM, true}, "Charles I");
    hierarchy.addMember({"Sophia", "05.03.1630",Religion::PROTESTANTISM, true}, "Elizabeth Stuart");

    hierarchy.addMember({"George I", "21.05.1660",Religion::PROTESTANTISM, true}, "Sophia");
    hierarchy.addMember({"George II", "22.06.1661",Religion::PROTESTANTISM, true}, "Sophia");

    SUBCASE("testPrintInfoFindsInfoForSearchedHeir") {
       std::string ans = hierarchy.printInfo("Charles II");
        CHECK(ans == "{\"name\" : \"Charles II\", \"birthdate\" : \"23.4.1630\", \"gender\" : \"male\", \"religion\" : \"protestant\"}");
    }

    SUBCASE("testPrintInfoThrowsInvalidArgumentWhenNoHeir") {
        CHECK_THROWS_AS(hierarchy.addMember({"John III", "25.08.1611", Religion::PROTESTANTISM, true}), const std::invalid_argument&);
    }
}

TEST_CASE("testDeclareDeceased") {
    Hierarchy hierarchy;
    SUBCASE("testDeclareDeceasedWhenEmptyHierarchy") {
        CHECK_THROWS_AS(hierarchy.declareDeceased("newName"), const EmptyHierarchyException&);
    }

    SUBCASE("testDeclareDeceasedHandlesCorrectlyMoreThanOneChildren") {
        std::vector<Heir> heirs = setUp();
        Heir heir = heirs[0];
        hierarchy.addMember(heir);
        hierarchy.addMember(heirs[1], "Pesho 0");

        for (std::size_t i = 2; i < 10; ++i) {
            std::string parent = heirs[i % 3].getName();
            hierarchy.addMember(heirs[i], heirs[i - 1].getName());
        }

        hierarchy.declareDeceased("Pesho 1");
        CHECK(!hierarchy.contains("Pesho 1"));

        Hierarchy::Iterator it = hierarchy.begin();
        CHECK(it->getName() == "Pesho 0");

        ++it;
        CHECK(it->getName() == "Pesho 2");
    }

    SUBCASE("testDeclareDeceasedMemberNotFound") {
        hierarchy = setHierarchy();
        CHECK_THROWS_AS(hierarchy.declareDeceased("Benedict X"), const MemberNotFoundException&);
    }
}

TEST_CASE("testFindSuccessors") {
    SUBCASE("testFindSuccessorsStraightLine") {

        Hierarchy hierarchy;
        std::vector<Heir> heirs = setUp();
        hierarchy.addMember(heirs[0]);

        for (std::size_t i = 1; i < heirs.size(); ++i) {
            hierarchy.addMember(heirs[i], heirs[i - 1].getName());
        }
        std::size_t n = 2;
        std::vector<Heir> ans = hierarchy.findSuccessors(n);

        Hierarchy hierarchyTestDifferentBirthdates;
        hierarchyTestDifferentBirthdates.addMember({"Pesho 0", "11.12.0999",Religion::PROTESTANTISM, true});
        hierarchyTestDifferentBirthdates.addMember({"Pesho 9", "11.3.1000", Religion::PROTESTANTISM, true}, "Pesho 0");

        for (std::size_t i = 10; i < 25; ++i) {
            hierarchyTestDifferentBirthdates.addMember({"Pesho " + std::to_string(i),
                                                        "11.2.10" + std::to_string(i),Religion::PROTESTANTISM, true},
                                                        "Pesho " + std::to_string(i - 1));
        }

        n = 10;
        ans.clear();
        ans = hierarchyTestDifferentBirthdates.findSuccessors(n);

        for (int i = 0; i < n - 1; ++i) {
            CHECK(ans[i + 1] < ans[i]);
        }
    }

    SUBCASE("testFindSuccessorsMoreThanOneBranch") {
        Hierarchy hierarchy;
        hierarchy.addMember({"James", "04.01.1231",Religion::PROTESTANTISM, true});
        hierarchy.addMember({"Charles", "05.02.1233",Religion::PROTESTANTISM, true}, "James");
        hierarchy.addMember({"William", "05.03.1234",Religion::PROTESTANTISM, true}, "James");

        std::vector<Heir> ans = hierarchy.findSuccessors(2);
        CHECK(ans[1] < ans[0]);

        hierarchy.addMember({"James II", "04.01.1236",Religion::PROTESTANTISM, true}, "Charles");
        ans.clear();
        ans = hierarchy.findSuccessors(3);
        std::size_t i = 0;
        std::vector<std::string> names = {"Charles", "James II", "William" };
        for (std::string name : names) {
            CHECK(name == ans[i++].getName());
        }

        hierarchy.addMember({"James III", "04.01.1236",Religion::PROTESTANTISM, true}, "Charles");
        names.clear();

        names = {"Charles", "James II", "James III", "William"};
        i = 0;
        ans.clear();
        ans = hierarchy.findSuccessors(4);
        for (std::string name : names) {
            CHECK(name == ans[i++].getName());
        }
    }

    SUBCASE("testFindSuccessorWomanAsSuccessor") {
        Hierarchy hierarchy = setHierarchy();

        std::vector<Heir> ans = hierarchy.findSuccessors(5);
        CHECK(ans[ans.size() - 1].getName() == "Elisabeth");
    }

    SUBCASE("testFindSuccessorsEmptySet") {
        Hierarchy h;
        CHECK_THROWS_AS(h.findSuccessors(12), const EmptyHierarchyException&);
    }
}

TEST_CASE("testEditReligionRemovesAllChildren") {
    Hierarchy hierarchy;

    hierarchy.addMember({"James III", "04.01.1236",Religion::PROTESTANTISM, true});
    std::vector<Heir> heirs = setUp();
    hierarchy.addMember(heirs[0], "James III");

    for (std::size_t i = 1; i < 50; ++i) {
        hierarchy.addMember(heirs[i], heirs[i - 1].getName());
    }

    SUBCASE("testEditReligionFindsMember") {
        hierarchy.editReligion("Pesho 0", Religion::CATHOLICISM);

        CHECK(!hierarchy.contains("Pesho 0"));
        CHECK(!hierarchy.contains("Pesho 12"));
        CHECK(hierarchy.contains("James III"));

        hierarchy.editReligion("James III", Religion::CATHOLICISM);
        CHECK(!hierarchy.contains("James III"));
    }

    SUBCASE("testEditReligionNoFoundMember") {
        CHECK_THROWS_AS(hierarchy.editReligion("Benedict X", Religion::CATHOLICISM), const MemberNotFoundException&);
    }

    SUBCASE("testEditReligionProtestantismNewReligion") {
        CHECK_THROWS_AS(hierarchy.editReligion("James III", Religion::PROTESTANTISM), const std::invalid_argument&);
    }
}

TEST_CASE("testMakeMonarch") {
    Hierarchy hierarchy = setHierarchy();
    SUBCASE("testMakeMonarchMonarchIsTheRoot") {
        CHECK_NOTHROW(hierarchy.makeMonarch("James"));
    }

    SUBCASE("testMakeMonarch") {
        hierarchy.makeMonarch("Charles");
        CHECK(!hierarchy.contains("James"));
        CHECK(hierarchy.getMonarch().getName() == "Charles");
    }

    SUBCASE("testMakeMonarchEmptyDataSet") {
        Hierarchy h;
        CHECK_THROWS_AS(h.makeMonarch(""), const EmptyHierarchyException&);
    }
}

TEST_CASE("testMakeSuccessorMonarch") {
    SUBCASE("testMakeSuccessorMonarch") {
        Hierarchy hierarchy = setHierarchy();
        hierarchy.makeSuccessorMonarch();
        CHECK(hierarchy.getMonarch().getName() == "Charles");
    }

    SUBCASE("testMakeSuccessorMonarchEmptySet") {
        Hierarchy hierarchy;

        CHECK_THROWS_AS(hierarchy.makeSuccessorMonarch(), const EmptyHierarchyException&);
    }
}

TEST_CASE("testIO") {
    SUBCASE("No Problem Expected") {
        const std::string filename = "test.txt";

        Hierarchy hierarchy = setHierarchy();
        hierarchy.save(filename);

        Hierarchy h;
        h.load(filename);
        Hierarchy::Iterator it = h.begin();

    for (Heir heir: hierarchy) {
        CHECK(heir == *it);
        ++it;
    }
}

    SUBCASE("testLoadThrowsExceptionWhenThereIsNoFile") {
        Hierarchy t;
        CHECK_NOTHROW_MESSAGE(t.load("non-existent-test.txt"), "Failed to open file with name: non-existent-test.txt");
    }

}

TEST_CASE("testMoveSemantics") {
    SUBCASE("testMovingConstructor") {
        Hierarchy h2 = setHierarchy();
        Hierarchy hierarchy(std::move(h2));
        CHECK(!(h2.begin() != h2.end()));
    }


    SUBCASE("testMoveOperator=") {
        Hierarchy h;
        Hierarchy h2 = setHierarchy();
        h = std::move(h2);
        CHECK(!(h2.begin() != h2.end()));
    }
}

