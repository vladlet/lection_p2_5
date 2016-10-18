//
// Created by Vladimir Letyago on 17/10/16.
//

#ifndef LECTION_P2_5_TESTCLASS_H
#define LECTION_P2_5_TESTCLASS_H

class TestClass {
public:
    TestClass(int val = 0);
    virtual ~TestClass();

    int getValue() const;
    void setValue(int val) ;

private:
    int m_value; ///< @brief Test value
};


#endif //LECTION_P2_5_TESTCLASS_H
