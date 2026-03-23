#include "machine_manager.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void test_remove_machine_operating(void) {
    MachManager manager = { .num_machines = 1 };
    Machine m;
    initialize_machine(&m, 1, "Corte");
    m.status = OP;
    manager.machines[0] = m;

    remove_machine(&manager, 1);
    CU_ASSERT_EQUAL(manager.num_machines, 1);
}

void test_remove_machine_not_operating(void) {
    MachManager manager = { .num_machines = 1 };
    Machine m;
    initialize_machine(&m, 1, "Corte");
    m.status = OFF;
    manager.machines[0] = m;

    remove_machine(&manager, 1);
    CU_ASSERT_EQUAL(manager.num_machines, 0);
}



int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Machine Manager Tests", 0, 0);

    CU_add_test(suite, "Test Remove Machine (In Operation)", test_remove_machine_operating);
    CU_add_test(suite, "Test Remove Machine (Not In Operation)", test_remove_machine_not_operating);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
