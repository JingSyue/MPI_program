FROM ubuntu:latest

RUN apt-get update && apt-get install -y mpich

WORKDIR /mpi_program
COPY logic_MPI.c /mpi_program
COPY prime_MPI.c /mpi_program
COPY floyd_MPI.c /mpi_program


RUN mpicc -o logic_MPI logic_MPI.c
RUN mpicc -o prime_MPI prime_MPI.c -lm
RUN mpicc -o floyd_MPI floyd_MPI.c