# MPI Programs

This repository contains MPI (Message Passing Interface) programs for solving problems using both parallel and sequential computing approaches. The problems covered include implementations for Floyd's algorithm, prime number calculation, and logic operations.

## Docker Environment

A `Dockerfile` is provided to create a suitable MPI environment with all necessary dependencies installed.

### Building the Docker Image

To build the Docker image, navigate to the directory containing the `Dockerfile` and run the following command:

```bash
docker build -t mpi_container .
```

# Running the MPI Programs

After building the image, you can run the MPI programs inside a Docker container.

## Floyd's Algorithm

To run the MPI version of Floyd's algorithm, use the following command:

```bash
docker run -it mpi_container mpirun -np [number_of_processes] ./floyd_MPI
```

## Prime Number Calculation
To calculate prime numbers using MPI, use this command:

```bash 
docker run -it mpi_container mpirun -np [number_of_processes] ./prime_MPI 'number of prime'
```

## Logic Operation
```bash
docker run mpi_container mpirun -np [number_of_processes] ./logic_MPI
```

