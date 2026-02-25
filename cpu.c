#include <stdio.h>
int stack[100];
int sp = -1;   // stack pointer
int memory[200];
int R[8];          // 4 Registers
int pc = 0;
int running = 1;

int ZF = 0;        // Zero Flag
int NF = 0;        // Negative Flag

void updateFlags(int result) {
    ZF = (result == 0);
    NF = (result < 0);
}

void execute() {
    while (running) {

        int opcode = memory[pc];
        int op1 = memory[pc + 1];
        int op2 = memory[pc + 2];

        switch (opcode) {

            case 1: // LOAD
                R[op1] = op2;
                updateFlags(R[op1]);
                pc += 3;
                break;

            case 2: // ADD
                R[op1] = R[op1] + R[op2];
                updateFlags(R[op1]);
                pc += 3;
                break;

            case 3: // SUB
                R[op1] = R[op1] - R[op2];
                updateFlags(R[op1]);
                pc += 3;
                break;

            case 4: // MUL
                R[op1] = R[op1] * R[op2];
                updateFlags(R[op1]);
                pc += 3;
                break;

            case 5: // DIV
                if (R[op2] != 0) {
                    R[op1] = R[op1] / R[op2];
                    updateFlags(R[op1]);
                }
                pc += 3;
                break;

            case 6: // PRINT
                printf("R%d = %d\n", op1, R[op1]);
                pc += 3;
                break;

            case 7: // AND
                R[op1] = R[op1] & R[op2];
                updateFlags(R[op1]);
                pc += 3;
                break;

            case 8: // OR
                R[op1] = R[op1] | R[op2];
                updateFlags(R[op1]);
                pc += 3;
                break;

            case 9: // XOR
                R[op1] = R[op1] ^ R[op2];
                updateFlags(R[op1]);
                pc += 3;
                break;

            case 10: // NOT
                R[op1] = ~R[op1];
                updateFlags(R[op1]);
                pc += 3;
                break;

            case 11: // CMP
                updateFlags(R[op1] - R[op2]);
                pc += 3;
                break;

            case 12: // JMP
                pc = op1;
                break;

            case 13: // JZ
                if (ZF)
                    pc = op1;
                else
                    pc += 3;
                break;

            case 14: // STORE
                memory[op2] = R[op1];
                pc += 3;
                break;

            case 15: // LOADM
                R[op1] = memory[op2];
                pc += 3;
                break;
                case 16: // PUSH
                stack[++sp] = R[op1];
                pc += 3;
                break;
                case 18: // CALL
                stack[++sp] = pc + 3;   // save return address
                pc = op1;
                break;

                case 19: // RET
                 pc = stack[sp--];
                 break;

            case 0: // HALT
                running = 0;
                break;

            default:
                printf("Invalid Instruction\n");
                running = 0;
        }
    }
}
int main() {

    int choice;
    int num1, num2;

    while(1) {

        printf("\n===== MINI CPU MENU =====\n");
        printf("1. Factorial\n");
        printf("2. Square\n");
        printf("3. Addition\n");
        printf("4. Subtraction\n");
        printf("5. Multiplication\n");
        printf("6. Division\n");
        printf("7. AND\n");
        printf("8. OR\n");
        printf("9. XOR\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if(choice == 0) {
            printf("Shutting down CPU...\n");
            break;
        }

        // Reset CPU state
        pc = 0;
        running = 1;
        for(int i=0;i<4;i++) R[i]=0;
        for(int i=0;i<200;i++) memory[i]=0;

        if(choice == 1) {
            printf("Enter number: ");
            scanf("%d", &num1);

            // Factorial
            memory[0]=1; memory[1]=1; memory[2]=1;      // R1 = 1
            memory[3]=1; memory[4]=2; memory[5]=num1;   // R2 = num
            memory[6]=1; memory[7]=3; memory[8]=1;      // R3 = 1

            int loop = 9;

            memory[9]=4; memory[10]=1; memory[11]=2;    // MUL
            memory[12]=3; memory[13]=2; memory[14]=3;   // SUB
            memory[15]=11; memory[16]=2; memory[17]=3;  // CMP
            memory[18]=13; memory[19]=24; memory[20]=0; // JZ END
            memory[21]=12; memory[22]=loop; memory[23]=0; // JMP LOOP
            memory[24]=6; memory[25]=1; memory[26]=0;   // PRINT
            memory[27]=0;
        }

        else if(choice >=2 && choice <=9) {

            printf("Enter first number: ");
            scanf("%d", &num1);
            printf("Enter second number: ");
            scanf("%d", &num2);

            memory[0]=1; memory[1]=0; memory[2]=num1;
            memory[3]=1; memory[4]=1; memory[5]=num2;

            int opcode;

            switch(choice) {
                case 2: opcode = 4; break; // Square handled below
                case 3: opcode = 2; break; // ADD
                case 4: opcode = 3; break; // SUB
                case 5: opcode = 4; break; // MUL
                case 6: opcode = 5; break; // DIV
                case 7: opcode = 7; break; // AND
                case 8: opcode = 8; break; // OR
                case 9: opcode = 9; break; // XOR
            }

            if(choice == 2) {
                // Square
                memory[3]=4; memory[4]=0; memory[5]=0;
                memory[6]=6; memory[7]=0; memory[8]=0;
                memory[9]=0;
            } else {
                memory[6]=opcode; memory[7]=0; memory[8]=1;
                memory[9]=6; memory[10]=0; memory[11]=0;
                memory[12]=0;
            }
        }

        execute();
    }

    return 0;
}