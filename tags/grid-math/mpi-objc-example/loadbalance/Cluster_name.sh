#PBS -S /bin/bash
#PBS -N PI 
#PBS -r n
#PBS -e PI.err 
#PBS -o PI.log
## -q indicates the queue; use short, long, or verylong
#PBS -q verylong
#PBS -l nodes=4:ppn=2
PBS_O_WORKDIR = ./
cd $PBS_O_WORKDIR
/home/local/mpich-1.2.5.2/bin/mpirun -v -machinefile $PBS_NODEFILE -np 4 ./lb > loadbalancer.out
