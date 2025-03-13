#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* regDecode(int key);
enum commandNames{
    add,
    sub,
    and,
    or,
    slt,
    addi,
    subi,
    load,
    store,
    beq,
    jump,
    jumpf
};
enum regNames{
    r0,
    rA,
    rB,
    rC
};

typedef struct {char *key; enum commandNames val;} commandKey;
typedef struct {char *key;enum regName val;} regKey;
typedef struct {char *symbol;char *address;} symbolAddressPair;

const char *commandNames[12] = {"add","sub","and","or","slt","addi","subi","load","store","beq","jump","jumpf"}; //array of pointers to characters
static commandKey commandTable[] = {{"add",add},{"sub",sub},
                                {"and",and},{"or",or},{"slt",slt},
                                {"addi",addi},{"subi",subi},
                                {"load",load},{"store",store},
                                {"beq",beq},{"jump",jump},
                                {"jumpf",jumpf}};
const char *regNames[4] = {"$r0","$rA","$rB","$rC"};
static regKey regTable[] = {{"$r0",r0},{"$rA",rA},{"$rB",rB},{"$rC",rC}};
#define numCommands (sizeof(commandTable)/sizeof(commandKey))
#define numRegisters (sizeof(regTable)/sizeof(regKey))



int enumFromString(char *cmd,int size,commandKey table[]){
    char *cmdInstr = strtok(cmd," ");
    int i;
    //printf(cmdInstr);
    for(i = 0;i<size;i++){
        //stringKey temp_sKey = lookupTable[i];
        if(strcmp(table[i].key,cmdInstr) == 0 ){
            return table[i].val;
       }
    }
    return -1;
}
//these functions could be combined into one by reusing the same struct for the registers
int regEnumFromString(char *reg,int size,regKey table[]){
    int i = 0;
    for(i=0;i<size;i++){
        if(strcmp(table[i].key,reg)==0){
            return table[i].val;
        }
    }
    return -1;
}
const char* rTypeInstruction(char *opcode,char *func,char *cmd){
    strtok(cmd," ");
    char *rs = strtok(NULL,",");
    int rsEnum = regEnumFromString(rs,numRegisters,regTable);
    const char *rsDecode = regDecode(rsEnum);
    char *rt = strtok(NULL,",");
    int rtEnum = regEnumFromString(rt,numRegisters,regTable);
    const char *rtDecode = regDecode(rtEnum);
    char *rd = strtok(NULL,";");
    int rdEnum = regEnumFromString(rd,numRegisters,regTable);
    const char *rdDecode = regDecode(rdEnum);

    char *fullCommand = "";
    strcpy(fullCommand,opcode);
    strcat(fullCommand,rsDecode);
    strcat(fullCommand,rtDecode);
    strcat(fullCommand,rdDecode);
    strcat(fullCommand,func);
    printf(fullCommand);
    printf("\n");
    return fullCommand;
    //printf(strtok(cmd," "));
    //char *rs =
}
const char* iTypeInstruction(char *opcode,char *cmd){
    strtok(cmd," ");
    char *rs = strtok(NULL,",");
    int rsEnum = regEnumFromString(rs,numRegisters,regTable);
    const char *rsDecode = regDecode(rsEnum);
    char *rd = strtok(NULL,",");
    int rdEnum = regEnumFromString(rd,numRegisters,regTable);
    const char *rdDecode = regDecode(rdEnum);
    char *immediate = strtok(NULL,";");
    
    bool errorMessage = false;
    int immediateDecimal =0;
    char *fullCommand = "";
    while(true){
        errno = 0;
        char *end;
        const long i = strtol(immediate,&end,10);
        if(immediate==end){
            break;
        }
        immediateDecimal = i;
        printf("integer extracted '%d'",i);
        const bool range_error = errno == ERANGE;
        printf("Extracted '%.*s', strtol returned %ld.", (int)(end-immediate), immediate, i);
        
        immediate = end;

 
        if (range_error){
            printf("\n --> Range error occurred.");
            errorMessage = true;
        }
        putchar('\n'); 
    }
    printf("Unextracted leftover: '%s'\n\n", immediate);
    printf("length of extract: %d",strlen(immediate));
    printf("Final Extraction: %d\n",immediateDecimal);
    if(strlen(immediate)!=0){
        errorMessage = true;
        printf("leftover immediate \n");
    }
    else{
        printf("no leftover immediate \n");
    }
    
    if(abs(immediateDecimal) > 255){
        errorMessage = true;
        printf("number magnitude is greater than 255 \n");
    }
    
    if(errorMessage == false){
        //printf("calculating the binary \n");
        //char immediateDecimalStr[6] = "";
        //strcpy(immediateDecimalStr," ");
        
        //printf("starting immediate decimal string is: %s\n",immediateDecimalStr);
        //printf("immediate decimal is %d \n",immediateDecimal);
        /*
        while(immediateDecimal>0){
            if(immediateDecimal%2){
                char *catStr = "1";
                strcat(immediateDecimalStr,catStr);
            }
            else{
                char *catStr ="0";
                strcat(immediateDecimalStr,catStr);
            }
            //printf("new immediate decimal string is: %s",immediateDecimalStr);
            immediateDecimal /= 2;
            //printf("\n");
        }*/
        char *binaryString = malloc(9*sizeof(int));
        unsigned int mask = 0b00000001;
        for(unsigned int i=0; i<8; i++)
        {
            
            binaryString[i] = (immediateDecimal & mask) ? '1' : '0';
            mask = mask << 1;
        }
        binaryString[8] = '\0';
        int c;
        
        strrev(binaryString);
        
        strcpy(fullCommand,opcode);
        strcat(fullCommand,rsDecode);
        strcat(fullCommand,rdDecode);
        strcat(fullCommand,binaryString);
        free(binaryString);
        printf(fullCommand);
        printf("\n");
    }
    else{
        fullCommand = "Error unexpected value";
    }
    
    return fullCommand;
}
const char* jumpTypeInstruction(char *opcode,char *cmd){
    strtok(cmd," ");
    char *rs = strtok(NULL,",");
    int rsEnum = regEnumFromString(rs,numRegisters,regTable);
    const char *rsDecode = regDecode(rsEnum);
    char *rd = strtok(NULL,";");
    int rdEnum = regEnumFromString(rd,numRegisters,regTable);
    const char *rdDecode = regDecode(rdEnum);
    char *fullCommand = "";

    strcpy(fullCommand,opcode);
    strcat(fullCommand,rsDecode);
    strcat(fullCommand,rdDecode);
    strcat(fullCommand,"00000000");

    return fullCommand;
}
const char* varHandlerInstruction(char* opcode,char* cmd,const char* address){
    printf("command inside var handler: %s \n",cmd);
    printf("inside of the var instruction\n");
    strtok(cmd," ");
    char *rs = strtok(NULL,",");
    int rsEnum = regEnumFromString(rs,numRegisters,regTable);
    const char *rsDecode = regDecode(rsEnum);
    char *rd = strtok(NULL,",");
    int rdEnum = regEnumFromString(rd,numRegisters,regTable);
    const char *rdDecode = regDecode(rdEnum);
    char *fullCommand = "";

    strcpy(fullCommand,opcode);
    strcat(fullCommand,rsDecode);
    strcat(fullCommand,rdDecode);
    strcat(fullCommand,address);
    return fullCommand;
}
const char* checkCmdForVar(char *cmd,symbolAddressPair varPairs[],int numberVars){
    printf("checking for var \n");
    strtok(cmd," ");
    strtok(NULL,",");
    strtok(NULL,",");
    
    char *immediate = strtok(NULL,";");
    printf("Printing the var address: %s",immediate);
    printf("\n");
    
    printf("size of the array in bytes: %d\n",sizeof(varPairs));
    printf("size of individual member: %d\n",sizeof(varPairs[0]));
    int size = sizeof(varPairs)/sizeof(varPairs[0]);//get the size
    printf("count of the array: %d",sizeof(varPairs)/sizeof(varPairs[0]));
    
    for(int i = 0;i<numberVars;i++){
        if(strcmp(immediate,varPairs[i].symbol) == 0){
            printf("found a command");
            const char *returnAddress = varPairs[i].address;
            return returnAddress;
        }
    }
    return NULL;
}
const char* regDecode(int key){
    switch(key){
        case r0:
            return "00";
            break;
        case rA:
            return "01";
            break;
        case rB:
            return "10";
            break;
        case rC:
            return "11";
            break; 
        default:
            return "not a valid command";
    }
}
const char* decode(int key,char *cmd,symbolAddressPair varPairList[],symbolAddressPair funcPairList[],int numberVars,int numberFunc,int currentMemoryAddress){
    char *opcode;
    char *func;
    const char *fullCommand;
    char *commandCopy = malloc(strlen(cmd)+1);
    const char *varCheck;
    printf("%d",key);
    switch(key){
        case add:
            opcode = "0000";
            func = "000010";
            printf("in here\n");
            fullCommand = rTypeInstruction(opcode,func,cmd);
            break;
        case sub:
            //printf("inside sub");
            opcode = "0000";
            func = "000011";
            fullCommand = rTypeInstruction(opcode,func,cmd);
            break;
        case and:
            opcode = "0000";
            func = "000000";
            fullCommand = rTypeInstruction(opcode,func,cmd);
            break;
        case or:
            opcode = "0000";
            func = "000001";
            fullCommand = rTypeInstruction(opcode,func,cmd);
            break;
        case slt:
            opcode = "0000";
            func = "000111";
            fullCommand = rTypeInstruction(opcode,func,cmd);
            break;
        case addi:
            opcode = "0001";
            fullCommand = iTypeInstruction(opcode,cmd);
            break;
        case subi:
            opcode = "0010";
            fullCommand = iTypeInstruction(opcode,cmd);
            break;
        case load:
            opcode = "0011";
            strcpy(commandCopy,cmd);
            
            varCheck = checkCmdForVar(cmd,varPairList,numberVars);
            if(varCheck!=NULL){
                fullCommand = varHandlerInstruction(opcode,commandCopy,varCheck);
            }
            else{
                printf("Executing i type instruction for store");
                fullCommand = iTypeInstruction(opcode,commandCopy);
            }
            
            break;
        case store:
            opcode = "0100";
            printf("Varpairlist0: %s\n",varPairList[0].symbol);
            printf("\n");
            printf("the command being checked: %s\n",cmd);
            strcpy(commandCopy,cmd);
            varCheck = checkCmdForVar(cmd,varPairList,numberVars);
            if(varCheck!=NULL){
                fullCommand = varHandlerInstruction(opcode,commandCopy,varCheck);
            }
            else{
                printf("Executing i type instruction for store");
                fullCommand = iTypeInstruction(opcode,commandCopy);
            }
           
            break;
        case beq:
            opcode = "0101";
            strcpy(commandCopy,cmd);
            varCheck = checkCmdForVar(cmd,funcPairList,numberFunc);
           
            if(varCheck!=NULL){
                strtok(commandCopy," ");
                int funcAddressInt = strtol(varCheck,NULL,2);
                int immediateAddress = funcAddressInt - currentMemoryAddress;
                int length = snprintf(NULL, 0,"%d",immediateAddress);
                char *immediateAddString = malloc((length+1)*sizeof(char));
                sprintf(immediateAddString,"%d",immediateAddress);
                char *rs = malloc(4*sizeof(char));
                strcpy(rs,strtok(NULL,","));
                //int rsEnum = regEnumFromString(rs,numRegisters,regTable);
                //const char *rsDecode = regDecode(rsEnum);
                char *rt = malloc(4*sizeof(char));
                strcpy(rt,strtok(NULL,",")); 
                //int rtEnum = regEnumFromString(rt,numRegisters,regTable);
                //const char *rtDecode = regDecode(rtEnum);
                free(commandCopy);
                commandCopy = malloc(strlen("beq ")+strlen(rs)+strlen(",")+strlen(rt)+strlen(",")+strlen(immediateAddString)+strlen(";")+1);
                strcpy(commandCopy,"beq ");
                strcat(commandCopy,rs);
                strcat(commandCopy,",");
                strcat(commandCopy,rt);
                strcat(commandCopy,",");
                strcat(commandCopy,immediateAddString);
                strcat(commandCopy,";");
                free(rs);
                free(rt);
                fullCommand = iTypeInstruction(opcode,commandCopy);
            }
            else{
                printf("Executing i type instruction for store");
                fullCommand = iTypeInstruction(opcode,commandCopy);
            }
            
            break;
        case jump:
            //printf("inside jump\n");
            opcode = "0110";
            fullCommand = jumpTypeInstruction(opcode,cmd);
            break;
        case jumpf:
            strcpy(commandCopy,cmd);
            strtok(cmd," ");
            char *rs = strtok(NULL,",");
            int rsEnum = regEnumFromString(rs,numRegisters,regTable);
            const char *rsDecode = regDecode(rsEnum);
            
            char *rt = strtok(NULL,",");
            int rtEnum = regEnumFromString(rt,numRegisters,regTable);
            const char *rtDecode = regDecode(rtEnum);

            opcode = "0001";
            char *command1 = malloc(17);
            const char *funcCheck = checkCmdForVar(commandCopy,funcPairList,numberFunc);
            char *binaryFuncAddress = malloc(17);
            
            char* funcCoderS = malloc(9);
            strncpy(funcCoderS, funcCheck+8, 8);
            funcCoderS[8] = '\0';
            sprintf(command1,"%s%s%s%s",opcode,"00",rsDecode,funcCoderS);
            free(funcCoderS);

            char *command2 = malloc(17);
            char* funcCoderT = malloc(9);
            strncpy(funcCoderT, funcCheck, 8);
            funcCoderT[8] = '\0';
            sprintf(command2,"%s%s%s%s",opcode,"00",rtDecode,funcCoderT);
            free(funcCoderT);
            opcode = "0110";
            char *command3 = malloc(17);
            sprintf(command3,"%s%s%s%s",opcode,rsDecode,rtDecode,"00000000");

            //command to add each 8 bit piece of the 16 bit memory address to the registers specified
            char* commandFinal = malloc(sizeof(command1)+sizeof(command2)+sizeof(command3)+2);
            strcpy(commandFinal,command1);
            strcat(commandFinal,"\n");
            strcat(commandFinal,command2);
            strcat(commandFinal,"\n");
            strcat(commandFinal,command3);
            fullCommand = commandFinal;
            //then exectute the jump command featuring the two registers
            //execute add command
            break;
        default:
            printf("not a valid command\n");
            fullCommand = "Command is unrecognizable";
            break;
    }
    free(commandCopy);
    return fullCommand;
}

void binaryToHex(char* hex,const char *binary){
    int hexInt = (int)strtol(binary,NULL,2);
    sprintf(hex, "%04x", hexInt);
}
void integerToBinaryString(char *binary, int decimal,int size){
    strcpy(binary,"");
    int index = 0;
    if(decimal==0){
        strcat(binary,"0");
    }
    while(decimal!=0 && index < size-1){
        if(decimal%2){
            strcat(binary,"1");
        }
        else{
            strcat(binary,"0");
        }
        decimal/=2;
        index++;
    }
    while(index<size-1){
        strcat(binary,"0");
        index++;
    }
    strrev(binary);
    snprintf(binary,size,"%s",binary);//clamps and adds null terminator
}
/*void varSearch(char *cmd,symbolAddressPair[] varTable){
    char *var = strtok(cmd,";");

}*/
int main(){
    //need to divide this into two separate passes
    
    FILE *filePtr = fopen("assemblycode.txt","r");
    FILE *outPtr = fopen("machinecode.txt","w");
    FILE *hexOutPtr = fopen("hexmachinecode.txt","w");
    int currentMemoryAddress = 0;
    char buff[30];
    int i = 0;
    
    int numberVars = 0;
    int numberFunc = 0;
    bool dataSection=false;
    //finds the amount of commands
    while(fgets(buff,sizeof(buff),filePtr)){
        char *command = buff;
        //printf(command); 
        if(strcmp(command,"@text\n") == 0){
            dataSection = false;
            printf("found test section \n");
        }
        else if(strcmp(command,"@data\n") == 0){
            printf("found the data command \n");
            dataSection = true;
        }
        else if(dataSection && strcmp(command,"\n") == 1){//this could use a more robust detection method
            numberVars++;
        }
        else if(!dataSection && buff[0]=='#'){
            numberFunc++;
        }
    }
    rewind(filePtr);
    printf("the number of vars in the file is: %d\n",numberVars);
    printf("the number of funcs in the file is: %d\n",numberFunc);
    
    symbolAddressPair *symbolList = (symbolAddressPair*)malloc(sizeof(symbolAddressPair*)*numberVars);//allocate memory for array
    symbolAddressPair *funcList = (symbolAddressPair*)malloc(sizeof(symbolAddressPair*)*numberFunc);//allocate memory for funcs

    if(symbolList ==NULL){
        printf("error with symbol list \n");
    }
    numberVars = 0;
    numberFunc = 0;
    while(fgets(buff,sizeof(buff),filePtr)){//stores variables with their memory locations
        char *command = buff;
        //printf(command); 
        if(strcmp(command,"@text\n") == 0){
            dataSection = false;
            printf("found test section \n");
        }
        else if(strcmp(command,"@data\n") == 0){
            printf("found the data command \n");
            dataSection = true;
        }
        else if(buff[0]!='@' && buff[0]!= '#' && strcmp(command,"\n")!=0 && !dataSection){
            if(!strncmp(buff,"jumpf",5)){
                currentMemoryAddress+=3;
            }
            else{
                currentMemoryAddress++;
            }
        }
        else if(dataSection && strcmp(command,"\n") == 1){//this could use a more robust detection method
            char *tempaddress = malloc(9);
            //sprintf(tempaddress,"%08x",numberVars);
            integerToBinaryString(tempaddress,numberVars,9);
            //strcat(tempaddress,"\0");
            symbolList[numberVars].address = (char *)malloc(9);//6 bits + 1 for null terminator
            strcpy(symbolList[numberVars].address,tempaddress);
            free(tempaddress);
            
            printf("storing command: %s",command);
            char* tempSymbol = strtok(command,";");
            int length = strlen(tempSymbol);
            //symbolAddressPair tempPair = {strtok(command,";"),tempaddress};
            char* symbol = malloc(length+1);
            strcpy(symbol,tempSymbol);
           // symbolList[numberVars].symbol = symbol;
            symbolList[numberVars].symbol = malloc(length+1);
            strcpy(symbolList[numberVars].symbol,symbol);
            free(symbol);
            printf("finished the copy \n");
            //symbolList[numberVars].symbol = strtok(command,";");
            //symbolList[numberVars].address = tempaddress;
            printf("stored in the list: %s at index: %d with address: %s",symbolList[numberVars].symbol,numberVars,symbolList[numberVars].address);
            numberVars++;
        }
        else if(!dataSection && buff[0]=='#'){ // designate each function as a 16 bit memory address
            char *funcName = strtok(command,"\n");
            funcName++;
            int length = strlen(funcName);
            char* func = malloc(length+1);
            strcpy(func,funcName);
            funcList[numberFunc].symbol = malloc(length+1);
            strcpy(funcList[numberFunc].symbol,func);
            free(func);
            printf("current function symbol in funclist: %s",funcList[numberFunc].symbol);
            char *tempaddress = malloc(17);
            //sprintf(tempaddress,"%016x",currentMemoryAddress);//instruction memory address is 16 bits
            integerToBinaryString(tempaddress,currentMemoryAddress,17);
            //strcat(tempaddress,"\0");
            funcList[numberFunc].address = (char *)malloc(17);//6 bits + 1 for null terminator
            strcpy(funcList[numberFunc].address,tempaddress);
            printf("current address in funclist: %s",funcList[numberFunc].address);
            free(tempaddress);
            numberFunc++;

        }
    }

    rewind(filePtr); 
    bool textSection = false;
    //converts commands
    currentMemoryAddress=0;
    fprintf(hexOutPtr,"v3.0 hex words addressed\n");//this line is for LOGISIM   
    while(fgets(buff,sizeof(buff),filePtr)){
        char *command = buff;
        printf(buff);
        printf("started main \n");

        if(strcmp(command,"@text\n") == 0){
            textSection = true;
            printf("found text section \n");
        }
        if(buff[0]!='@' && buff[0]!= '#' && strcmp(command,"\n")!=0 && textSection){
            char *commandTemp = "";
            strcpy(commandTemp,command); 
            int key = enumFromString(commandTemp,numCommands,commandTable);
            //printf("%d",key);
            printf("Immediately before decode: %s",symbolList[0].symbol);
            const char *code = decode(key,command,symbolList,funcList,numberVars,numberFunc,currentMemoryAddress);
            printf("heres the code: %s \n",code);//break up pieces of the string and increment
            char *tempCode = malloc(strlen(code)+1);
            strcpy(tempCode,code);
            char *slicedTempcode = strtok(tempCode,"\n");
            while(slicedTempcode!=NULL){
                char hex[4];
                binaryToHex(hex,slicedTempcode);
                //printf("hexcode: %s \n",hex);
                fprintf(outPtr,"%s\n",slicedTempcode);
                fprintf(hexOutPtr,"%04x : %s\n",currentMemoryAddress,hex);
                currentMemoryAddress++;
                slicedTempcode = strtok(NULL,"\n");
            }
            free(tempCode);
        }
    }
    //printf("%d",code);
    fclose(filePtr);
    fclose(outPtr);
    fclose(hexOutPtr);
    for(int i =0;i<numberVars;i++){
        free(symbolList[i].symbol);
        free(symbolList[i].address);
    }
    for(int i =0;i<numberFunc;i++){
        free(funcList[i].symbol);
        free(funcList[i].address);
    }
    free(symbolList);
    free(funcList);
    return 0;
}
