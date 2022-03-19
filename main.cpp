#include <iostream>
#include <string.h>
#include "libpq-fe.h" // Lib necessária

using namespace std;

// Compilação
//g++ -o main -I /usr/include/postgresql/ main.cpp -L /usr/lib/ -lpq

// CRUD
void CREATE(PGconn *conn, const char* query);
void READ(PGconn *conn, const char* query);
void UPDATE(PGconn *conn, const char* query);
void DELETE(PGconn *conn, const char* query);

int main()
{
    // Declara uma variavel logica que representara o banco de dados
    PGconn *conn = NULL;
    // Cria uma conexao com um banco do PostgreSQL
    conn = PQconnectdb ("host=localhost port=5432 dbname=postgres user=postgres password=marcusv77");

    // Verifica a conexao feita
    if (PQstatus(conn) != CONNECTION_OK){
        // Retorna uma mensagem de erro do PostgreSQL
        cout << PQerrorMessage(conn);
        
        // Encerra a conexao com o banco de dados
        PQfinish(conn);
        return 1;
    }
    else cout << "Conexao executada com sucesso!" << endl << endl;

    // Realizando a consulta Número 01
    cout << "Consultando se há um jogo com valor <= a R$15.00." << endl;
    READ(conn, "select Acessivel.nome from Jogo as Acessivel where valor<=15.00;");

    // Realizando a consulta Número 02
    cout << "\nConsultando se há um aluguel com valor <= a R$15.00." << endl;
    READ(conn, "select * from Aluguel where valor<=15.00;");

    // Realizando a consulta Número 03
    cout << "\nConsultando se há um desenvolvedor com CPF = a 578.847.812-04." << endl;
    READ(conn, "select * from Desenvolvedor where cpf='578.847.812-04';");

    // Realizando a consulta Número 04 (COMPLEXA)
    cout << "\nConsultando Alugueis com valor total >= 14.00." << endl;
    READ(conn, "select P.nome, E.Numero, E.CodigoJogo from Usuario as U, Aluguel as A, Pessoa as P, Exemplar as E where (E.Numero=A.NumeroExemplar) and (E.CodigoJogo=A.CodigoJogo) and (U.Cpf=P.Cpf) and (A.CpfUsuario=U.Cpf) and (A.Valor >= 14.00);");

    // Inserindo uma nova pessoa
    cout << "\nCriando uma pessoa com CPF = a 444.444.444-44 e nome = a Gusttavo Lima." << endl;
    CREATE(conn, "insert into Pessoa values ('444.444.444-44', 'Gusttavo Lima');");

    // Realizando a atualização Número 01
    cout << "\nAtualizando a pessoa com CPF = a 444.444.444-44." << endl;
    UPDATE(conn, "update Pessoa set nome='Nivaldo Batista Lima' where cpf='444.444.444-44';");

    // Realizando a remoção Número 01
    cout << "\nApagando a pessoa cujo CPF = 444.444.444-44." << endl;
    DELETE(conn, "delete from Pessoa where cpf='444.444.444-44';");

    // Encerra a conexao com o banco de dados
    PQfinish(conn);

    return 0;
}

void CREATE(PGconn *conn, const char* query){
    char* nTuplas;
    
    PGresult *res;
    res = PQexec(conn, query);
    
    // Verifica a validade da consulta
    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        cout << PQerrorMessage(conn);
        PQclear(res);
        PQfinish(conn);
        exit (2);
    }

    // Obtem o numero de tuplas
    nTuplas = PQcmdTuples(res);
    cout << "Número de tuplas alteradas no INSERT: " << nTuplas << endl;

    // Fecha o acesso ao resultado da consulta
    PQclear(res);

}

void READ(PGconn *conn, const char* query){
    int nTuplas, nAtributos;

    PGresult *res;
    res = PQexec(conn, query);
    
    // Verifica a validade da consulta
    if (PQresultStatus(res) != PGRES_TUPLES_OK){
        cout << PQerrorMessage(conn);
        PQclear(res);
        PQfinish(conn);
        exit (2);
    }

    // Obtem o numero de tuplas
    nTuplas = PQntuples(res);
    // Obtem o numero de atributos
    nAtributos = PQnfields(res);

    // Percorre todos as tuplas
    for (int i = 0; i < nTuplas; i++) {
        // Percorre todos os atributos
        for (int j = 0; j < nAtributos; j++){
            // Imprime o valor do atributo j da tupla i
            cout << PQgetvalue(res, i, j) << "\t";
        }
        cout << endl;
    }

    // Fecha o acesso ao resultado da consulta
    PQclear(res);
    
}

void UPDATE(PGconn *conn, const char* query){
    char* nTuplas;
    
    PGresult *res;
    res = PQexec(conn, query);
    
    // Verifica a validade da consulta
    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        cout << PQerrorMessage(conn);
        PQclear(res);
        PQfinish(conn);
        exit (2);
    }

    // Obtem o numero de tuplas
    nTuplas = PQcmdTuples(res);
    cout << "Número de tuplas alteradas no UPDATE: " << nTuplas << endl;

    // Fecha o acesso ao resultado da consulta
    PQclear(res);
}

void DELETE(PGconn *conn, const char* query){
    char* nTuplas;
    
    PGresult *res;
    res = PQexec(conn, query);
    
    // Verifica a validade da consulta
    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        cout << PQerrorMessage(conn);
        PQclear(res);
        PQfinish(conn);
        exit (2);
    }

    // Obtem o numero de tuplas
    nTuplas = PQcmdTuples(res);
    cout << "Número de tuplas alteradas no DELETE: " << nTuplas << endl;
    
    // Fecha o acesso ao resultado da consulta
    PQclear(res);

}