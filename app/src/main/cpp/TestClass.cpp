//
// Created by Vladimir Letyago on 17/10/16.
//

#include "TestClass.h"


TestClass::TestClass(int val /* =0 */):
        m_value(val) {
}

TestClass::~TestClass() {

}

int TestClass::getValue() const {
    return m_value;
}

void TestClass::setValue(int val) {
    m_value = val;
}


