#! /bin/sh
# -*- shell-script -*-
#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

# Do a little error checking

if test ! -r "$1/fortran_kinds.sh"; then
    echo "ERROR: Cannot find $1/fortran_kinds.sh"
    exit 1
elif test ! -s "$1/fortran_kinds.sh"; then
    echo "ERROR: $1/fortran_kinds.sh appears to be empty!"
    exit 1
fi

# Read the setup information

echo "Reading Fortran KIND information..." >&2
. "$1/fortran_kinds.sh"

# Do we want the 2 buffer functions?  (see comment in
# ompi/mpi/f90/Makefile.am)

WANT_2BUFFER_FUNCTIONS=$2
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then
    echo "Generating 2 buffer function interfaces: yes" >&2
else
    echo "Generating 2 buffer function interfaces: no" >&2
fi

# Do the work

echo "Generating F90 interface functions..." >&2

# Setup

rank=2
array_ranks="1"
while test "`expr $rank \\<= $max_array_rank`" = "1"; do
    array_ranks="$array_ranks $rank"
    rank="`expr $rank + 1`"
done

# Subroutine helper used to make dimension strings

make_dims() {
    dim=":"
    j=2
    while test "`expr $j \\<= $rank`" = "1"; do
        dim="$dim,:"
        j="`expr $j + 1`"
    done
}

# Work for each procedure

procedure='MPI_Abort'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, errorcode, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: errorcode"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Accumulate'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, op, win, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, op, win, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, op, win, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, op, win, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, op, win, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, op, win, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, op, win, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, op, win, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Add_error_class'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(errorclass, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: errorclass"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Add_error_code'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(errorclass, errorcode, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: errorclass"
echo "  integer, intent(out) :: errorcode"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Add_error_string'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(errorcode, string, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: errorcode"
echo "  character(len=*), intent(in) :: string"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Address'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(location, address, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: location"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(location, address, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: location"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(location, address, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: location"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(location, address, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: location"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(location, address, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: location"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(location, address, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: location"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(location, address, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: location"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(location, address, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: location"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Allgather'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Allgatherv'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: displs"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: displs"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: displs"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: displs"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: displs"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: displs"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: displs"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: displs"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

procedure='MPI_Alloc_mem'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(size, info, baseptr, ierr)"
echo "  use mpi_kinds"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: size"
echo "  integer, intent(in) :: info"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: baseptr"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Allreduce'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Alltoall'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Alltoallv'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtype, recvbuf, &
        recvcounts, rdispls, recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, dimension(*), intent(in) :: sendcounts"
  echo "  integer, dimension(*), intent(in) :: sdispls"
  echo "  integer, intent(in) :: sendtype"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: rdispls"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtype, recvbuf, &
        recvcounts, rdispls, recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, dimension(*), intent(in) :: sendcounts"
  echo "  integer, dimension(*), intent(in) :: sdispls"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: rdispls"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtype, recvbuf, &
        recvcounts, rdispls, recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, dimension(*), intent(in) :: sendcounts"
  echo "  integer, dimension(*), intent(in) :: sdispls"
  echo "  integer, intent(in) :: sendtype"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: rdispls"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtype, recvbuf, &
        recvcounts, rdispls, recvtype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, dimension(*), intent(in) :: sendcounts"
  echo "  integer, dimension(*), intent(in) :: sdispls"
  echo "  integer, intent(in) :: sendtype"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: rdispls"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtype, recvbuf, &
        recvcounts, rdispls, recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, dimension(*), intent(in) :: sendcounts"
    echo "  integer, dimension(*), intent(in) :: sdispls"
    echo "  integer, intent(in) :: sendtype"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: rdispls"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtype, recvbuf, &
        recvcounts, rdispls, recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, dimension(*), intent(in) :: sendcounts"
    echo "  integer, dimension(*), intent(in) :: sdispls"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: rdispls"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtype, recvbuf, &
        recvcounts, rdispls, recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, dimension(*), intent(in) :: sendcounts"
    echo "  integer, dimension(*), intent(in) :: sdispls"
    echo "  integer, intent(in) :: sendtype"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: rdispls"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtype, recvbuf, &
        recvcounts, rdispls, recvtype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, dimension(*), intent(in) :: sendcounts"
    echo "  integer, dimension(*), intent(in) :: sdispls"
    echo "  integer, intent(in) :: sendtype"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: rdispls"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Alltoallw'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, &
        recvcounts, rdispls, recvtypes, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, dimension(*), intent(in) :: sendcounts"
  echo "  integer, dimension(*), intent(in) :: sdispls"
  echo "  integer, dimension(*), intent(in) :: sendtypes"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: rdispls"
  echo "  integer, dimension(*), intent(in) :: recvtypes"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, &
        recvcounts, rdispls, recvtypes, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, dimension(*), intent(in) :: sendcounts"
  echo "  integer, dimension(*), intent(in) :: sdispls"
  echo "  integer, dimension(*), intent(in) :: sendtypes"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: rdispls"
  echo "  integer, dimension(*), intent(in) :: recvtypes"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, &
        recvcounts, rdispls, recvtypes, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, dimension(*), intent(in) :: sendcounts"
  echo "  integer, dimension(*), intent(in) :: sdispls"
  echo "  integer, dimension(*), intent(in) :: sendtypes"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: rdispls"
  echo "  integer, dimension(*), intent(in) :: recvtypes"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, &
        recvcounts, rdispls, recvtypes, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, dimension(*), intent(in) :: sendcounts"
  echo "  integer, dimension(*), intent(in) :: sdispls"
  echo "  integer, dimension(*), intent(in) :: sendtypes"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: rdispls"
  echo "  integer, dimension(*), intent(in) :: recvtypes"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, &
        recvcounts, rdispls, recvtypes, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, dimension(*), intent(in) :: sendcounts"
    echo "  integer, dimension(*), intent(in) :: sdispls"
    echo "  integer, dimension(*), intent(in) :: sendtypes"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: rdispls"
    echo "  integer, dimension(*), intent(in) :: recvtypes"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, &
        recvcounts, rdispls, recvtypes, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, dimension(*), intent(in) :: sendcounts"
    echo "  integer, dimension(*), intent(in) :: sdispls"
    echo "  integer, dimension(*), intent(in) :: sendtypes"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: rdispls"
    echo "  integer, dimension(*), intent(in) :: recvtypes"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, &
        recvcounts, rdispls, recvtypes, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, dimension(*), intent(in) :: sendcounts"
    echo "  integer, dimension(*), intent(in) :: sdispls"
    echo "  integer, dimension(*), intent(in) :: sendtypes"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: rdispls"
    echo "  integer, dimension(*), intent(in) :: recvtypes"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, &
        recvcounts, rdispls, recvtypes, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, dimension(*), intent(in) :: sendcounts"
    echo "  integer, dimension(*), intent(in) :: sdispls"
    echo "  integer, dimension(*), intent(in) :: sendtypes"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: rdispls"
    echo "  integer, dimension(*), intent(in) :: recvtypes"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

procedure='MPI_Attr_delete'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, keyval, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: keyval"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Attr_get'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, keyval, attribute_val, flag, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: keyval"
echo "  integer, intent(out) :: attribute_val"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Attr_put'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, keyval, attribute_val, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: keyval"
echo "  integer, intent(in) :: attribute_val"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Barrier'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Bcast'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buffer, count, datatype, root, comm&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buffer"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buffer, count, datatype, root, comm&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buffer"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buffer, count, datatype, root, comm&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buffer"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buffer, count, datatype, root, comm&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buffer"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buffer, count, datatype, root, comm&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buffer"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buffer, count, datatype, root, comm&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buffer"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buffer, count, datatype, root, comm&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buffer"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buffer, count, datatype, root, comm&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buffer"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Bsend'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Bsend_init'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Buffer_attach'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buffer, size, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buffer"
  echo "  integer, intent(in) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buffer, size, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buffer"
  echo "  integer, intent(in) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buffer, size, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buffer"
  echo "  integer, intent(in) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buffer, size, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buffer"
  echo "  integer, intent(in) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buffer, size, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buffer"
    echo "  integer, intent(in) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buffer, size, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buffer"
    echo "  integer, intent(in) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buffer, size, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buffer"
    echo "  integer, intent(in) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buffer, size, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buffer"
    echo "  integer, intent(in) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Buffer_detach'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buffer, size, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buffer"
  echo "  integer, intent(out) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buffer, size, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buffer"
  echo "  integer, intent(out) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buffer, size, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buffer"
  echo "  integer, intent(out) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buffer, size, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buffer"
  echo "  integer, intent(out) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buffer, size, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buffer"
    echo "  integer, intent(out) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buffer, size, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buffer"
    echo "  integer, intent(out) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buffer, size, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buffer"
    echo "  integer, intent(out) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buffer, size, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buffer"
    echo "  integer, intent(out) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Cancel'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(request, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: request"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Cart_coords'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, rank, maxdims, coords, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: rank"
echo "  integer, intent(in) :: maxdims"
echo "  integer, dimension(*), intent(out) :: coords"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Cart_create'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(old_comm, ndims, dims, periods, reorder, &
        comm_cart, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: old_comm"
echo "  integer, intent(in) :: ndims"
echo "  integer, dimension(*), intent(in) :: dims"
echo "  logical, dimension(*), intent(in) :: periods"
echo "  logical, intent(in) :: reorder"
echo "  integer, intent(out) :: comm_cart"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Cart_get'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, maxdims, dims, periods, coords&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: maxdims"
echo "  integer, dimension(*), intent(out) :: dims"
echo "  logical, dimension(*), intent(out) :: periods"
echo "  integer, dimension(*), intent(out) :: coords"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Cart_map'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, ndims, dims, periods, newrank&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: ndims"
echo "  integer, dimension(*), intent(in) :: dims"
echo "  logical, dimension(*), intent(in) :: periods"
echo "  integer, intent(out) :: newrank"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Cart_rank'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, coords, rank, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, dimension(*), intent(in) :: coords"
echo "  integer, intent(out) :: rank"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Cart_shift'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, direction, disp, rank_source, rank_dest&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: direction"
echo "  integer, intent(in) :: disp"
echo "  integer, intent(out) :: rank_source"
echo "  integer, intent(out) :: rank_dest"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Cart_sub'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, remain_dims, new_comm, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  logical, dimension(*), intent(in) :: remain_dims"
echo "  integer, intent(out) :: new_comm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Cartdim_get'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, ndims, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: ndims"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_call_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, errorcode, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: errorcode"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_compare'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm1, comm2, result, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm1"
echo "  integer, intent(in) :: comm2"
echo "  integer, intent(out) :: result"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_create'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, group, newcomm, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: group"
echo "  integer, intent(out) :: newcomm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_create_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(function, errhandler, ierr)"
echo "  use mpi_kinds"
echo "  external :: function"
echo "  integer, intent(out) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_create_keyval'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm_copy_attr_fn, comm_delete_attr_fn, comm_keyval, extra_state, ierr)"
echo "  use mpi_kinds"
echo "  external :: comm_copy_attr_fn"
echo "  external :: comm_delete_attr_fn"
echo "  integer, intent(out) :: comm_keyval"
echo "  integer, intent(in) :: extra_state"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_delete_attr'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, comm_keyval, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: comm"
echo "  integer, intent(in) :: comm_keyval"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_dup'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, newcomm, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: newcomm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_free'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: comm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_free_keyval'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm_keyval, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: comm_keyval"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_get_attr'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, comm_keyval, attribute_val, flag, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: comm_keyval"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: attribute_val"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_get_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, erhandler, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: erhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_get_name'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, comm_name, resultlen, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  character(len=*), intent(out) :: comm_name"
echo "  integer, intent(out) :: resultlen"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_group'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, group, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: group"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_rank'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, rank, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: rank"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_remote_group'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, group, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: group"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_remote_size'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, size, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: size"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_set_attr'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, comm_keyval, attribute_val, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: comm"
echo "  integer, intent(in) :: comm_keyval"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: attribute_val"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_set_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, errhandler, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: comm"
echo "  integer, intent(in) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_set_name'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, comm_name, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: comm"
echo "  character(len=*), intent(in) :: comm_name"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_size'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, size, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: size"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_split'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, color, key, newcomm, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: color"
echo "  integer, intent(in) :: key"
echo "  integer, intent(out) :: newcomm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_test_inter'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, flag, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: comm"
echo "  logical, intent(in) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Dims_create'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(nnodes, ndims, dims, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: nnodes"
echo "  integer, intent(in) :: ndims"
echo "  integer, dimension(*), intent(inout) :: dims"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Errhandler_create'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(function, errhandler, ierr)"
echo "  use mpi_kinds"
echo "  external :: function"
echo "  integer, intent(out) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Errhandler_free'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(errhandler, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Errhandler_get'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, errhandler, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Errhandler_set'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, errhandler, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Error_class'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(errorcode, errorclass, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: errorcode"
echo "  integer, intent(out) :: errorclass"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Error_string'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(errorcode, string, resultlen, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: errorcode"
echo "  character(len=*), intent(out) :: string"
echo "  integer, intent(out) :: resultlen"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Exscan'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

procedure='MPI_File_call_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, errorcode, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  integer, intent(in) :: errorcode"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_close'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: fh"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_create_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(function, errhandler, ierr)"
echo "  use mpi_kinds"
echo "  external :: function"
echo "  integer, intent(out) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_delete'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(filename, info, ierr)"
echo "  use mpi_kinds"
echo "  character(len=*), intent(in) :: filename"
echo "  integer, intent(in) :: info"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_get_amode'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, amode, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  integer, intent(out) :: amode"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_get_atomicity'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, flag, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_get_byte_offset'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, offset, disp, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
echo "  integer(kind=MPI_OFFSET_KIND), intent(out) :: disp"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_get_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(file, errhandler, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: file"
echo "  integer, intent(out) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_get_group'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, group, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  integer, intent(out) :: group"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_get_info'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, info_used, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  integer, intent(out) :: info_used"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_get_position'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, offset, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  integer(kind=MPI_OFFSET_KIND), intent(out) :: offset"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_get_position_shared'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, offset, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  integer(kind=MPI_OFFSET_KIND), intent(out) :: offset"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_get_size'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, size, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  integer(kind=MPI_OFFSET_KIND), intent(out) :: size"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_get_type_extent'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, datatype, extent, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  integer, intent(in) :: datatype"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: extent"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_get_view'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, disp, etype, filetype, datarep&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  integer(kind=MPI_OFFSET_KIND), intent(out) :: disp"
echo "  integer, intent(out) :: etype"
echo "  integer, intent(out) :: filetype"
echo "  character(len=*), intent(out) :: datarep"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_iread'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_iread_at'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_iread_shared'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_iwrite'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_iwrite_at'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_iwrite_shared'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_open'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, filename, amode, info, fh&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  character(len=*), intent(in) :: filename"
echo "  integer, intent(in) :: amode"
echo "  integer, intent(in) :: info"
echo "  integer, intent(out) :: fh"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_preallocate'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, size, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: fh"
echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: size"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read_all'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read_all_begin'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read_all_end'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read_at'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read_at_all'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read_at_all_begin'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read_at_all_end'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(in) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(in) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read_ordered'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read_ordered_begin'

echo "interface ${procedure}"
echo
rank=0

# JMS: Override -- need to make the back-end function less than 31
# characters because F90 standard says that symbol max lengths are 31
# characters.  So change the value of $procedure to something slightly
# shorter.  $proc, hard code it to something slightly different.
# #$%@#$%@#$%
procedure=MPI_File_read_ord_begin

for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
# JMS: Override -- see above.  Put it back to the full name.
# #$%@#$%@#$%
procedure=MPI_File_read_ordered_begin
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read_ordered_end'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_read_shared'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_seek'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, offset, whence, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: fh"
echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
echo "  integer, intent(in) :: whence"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_seek_shared'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, offset, whence, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: fh"
echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
echo "  integer, intent(in) :: whence"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_set_atomicity'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, flag, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: fh"
echo "  logical, intent(in) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_set_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(file, errhandler, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: file"
echo "  integer, intent(in) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_set_info'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, info, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: fh"
echo "  integer, intent(in) :: info"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_set_size'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, size, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: fh"
echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: size"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_set_view'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, disp, etype, filetype, datarep, &
        info, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fh"
echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: disp"
echo "  integer, intent(in) :: etype"
echo "  integer, intent(in) :: filetype"
echo "  character(len=*), intent(in) :: datarep"
echo "  integer, intent(in) :: info"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_sync'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fh, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: fh"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write_all'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write_all_begin'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write_all_end'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write_at'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write_at_all'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype, &
        status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write_at_all_begin'

echo "interface ${procedure}"
echo
rank=0

# JMS: Override -- need to make the back-end function less than 31
# characters because F90 standard says that symbol max lengths are 31
# characters.  So change the value of $procedure to something slightly
# shorter.  $proc, hard code it to something slightly different.
# #$%@#$%@#$%
procedure=MPI_File_wr_at_all_begin

for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, offset, buf, count, datatype&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_OFFSET_KIND), intent(in) :: offset"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
# JMS: Override -- see above.  Put it back to the full name.
# #$%@#$%@#$%
procedure=MPI_File_write_at_all_begin
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write_at_all_end'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write_ordered'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write_ordered_begin'

echo "interface ${procedure}"
echo
rank=0


# JMS: Override -- need to make the back-end function less than 31
# characters because F90 standard says that symbol max lengths are 31
# characters.  So change the value of $procedure to something slightly
# shorter.  $proc, hard code it to something slightly different.
# #$%@#$%@#$%
procedure=MPI_File_wr_ord_begin
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
# JMS: Override -- see above.  Put it back to the full name.
# #$%@#$%@#$%
procedure=MPI_File_write_ordered_begin
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write_ordered_end'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_File_write_shared'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
  echo "  use mpi_kinds"
  echo "  integer, intent(inout) :: fh"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(fh, buf, count, datatype, status&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(out) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Finalize'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Finalized'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(flag, ierr)"
echo "  use mpi_kinds"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Free_mem'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(base, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: base"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(base, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: base"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(base, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: base"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(base, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: base"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(base, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: base"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(base, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: base"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(base, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: base"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(base, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: base"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Gather'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Gatherv'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: displs"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: displs"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: displs"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, dimension(*), intent(in) :: recvcounts"
  echo "  integer, dimension(*), intent(in) :: displs"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: displs"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: displs"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: displs"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcounts, &
        displs, recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, dimension(*), intent(in) :: recvcounts"
    echo "  integer, dimension(*), intent(in) :: displs"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

procedure='MPI_Get'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Get_address'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(location, address, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: location"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(location, address, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: location"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(location, address, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: location"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(location, address, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: location"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(location, address, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: location"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(location, address, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: location"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(location, address, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: location"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(location, address, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: location"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: address"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Get_count'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(status, datatype, count, ierr)"
echo "  use mpi_kinds"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(in) :: status"
echo "  integer, intent(in) :: datatype"
echo "  integer, intent(out) :: count"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Get_elements'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(status, datatype, count, ierr)"
echo "  use mpi_kinds"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(in) :: status"
echo "  integer, intent(in) :: datatype"
echo "  integer, intent(out) :: count"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Get_processor_name'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(name, resultlen, ierr)"
echo "  use mpi_kinds"
echo "  character(len=*), intent(out) :: name"
echo "  integer, intent(out) :: resultlen"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Get_version'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(version, subversion, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(out) :: version"
echo "  integer, intent(out) :: subversion"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Graph_create'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm_old, nnodes, index, edges, reorder, &
        comm_graph, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm_old"
echo "  integer, intent(in) :: nnodes"
echo "  integer, dimension(*), intent(in) :: index"
echo "  integer, dimension(*), intent(in) :: edges"
echo "  logical, intent(in) :: reorder"
echo "  integer, intent(out) :: comm_graph"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Graph_get'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, maxindex, maxedges, index, edges&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: maxindex"
echo "  integer, intent(in) :: maxedges"
echo "  integer, dimension(*), intent(out) :: index"
echo "  integer, dimension(*), intent(out) :: edges"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Graph_map'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, nnodes, index, edges, newrank&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: nnodes"
echo "  integer, dimension(*), intent(in) :: index"
echo "  integer, dimension(*), intent(in) :: edges"
echo "  integer, intent(out) :: newrank"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Graph_neighbors'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, rank, maxneighbors, neighbors, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: rank"
echo "  integer, intent(in) :: maxneighbors"
echo "  integer, dimension(*), intent(out) :: neighbors"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Graph_neighbors_count'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, rank, nneighbors, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(in) :: rank"
echo "  integer, intent(out) :: nneighbors"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Graphdims_get'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, nnodes, nedges, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: nnodes"
echo "  integer, intent(out) :: nedges"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Grequest_complete'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(request, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: request"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Grequest_start'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(query_fn, free_fn, cancel_fn, extra_state, request&
        , ierr)"
echo "  use mpi_kinds"
echo "  external :: query_fn"
echo "  external :: free_fn"
echo "  external :: cancel_fn"
echo "  integer, intent(in) :: extra_state"
echo "  integer, intent(out) :: request"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_compare'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group1, group2, result, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group1"
echo "  integer, intent(in) :: group2"
echo "  integer, intent(out) :: result"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_difference'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group1, group2, newgroup, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group1"
echo "  integer, intent(in) :: group2"
echo "  integer, intent(out) :: newgroup"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_excl'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group, n, ranks, newgroup, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group"
echo "  integer, intent(in) :: n"
echo "  integer, dimension(*), intent(in) :: ranks"
echo "  integer, intent(out) :: newgroup"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_free'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: group"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_incl'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group, n, ranks, newgroup, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group"
echo "  integer, intent(in) :: n"
echo "  integer, dimension(*), intent(in) :: ranks"
echo "  integer, intent(out) :: newgroup"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_intersection'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group1, group2, newgroup, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group1"
echo "  integer, intent(in) :: group2"
echo "  integer, intent(out) :: newgroup"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_range_excl'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group, n, ranges, newgroup, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group"
echo "  integer, intent(in) :: n"
echo "  integer, dimension(3, *), intent(in) :: ranges"
echo "  integer, intent(out) :: newgroup"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_range_incl'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group, n, ranges, newgroup, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group"
echo "  integer, intent(in) :: n"
echo "  integer, dimension(3, *), intent(in) :: ranges"
echo "  integer, intent(out) :: newgroup"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_rank'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group, rank, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group"
echo "  integer, intent(out) :: rank"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_size'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group, size, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group"
echo "  integer, intent(out) :: size"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_translate_ranks'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group1, n, ranks1, group2, ranks2&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group1"
echo "  integer, intent(in) :: n"
echo "  integer, dimension(*), intent(in) :: ranks1"
echo "  integer, intent(in) :: group2"
echo "  integer, dimension(*), intent(out) :: ranks2"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Group_union'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group1, group2, newgroup, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group1"
echo "  integer, intent(in) :: group2"
echo "  integer, intent(out) :: newgroup"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Ibsend'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Info_create'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(info, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(out) :: info"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Info_delete'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(info, key, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(out) :: info"
echo "  character(len=*), intent(in) :: key"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Info_dup'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(info, newinfo, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: info"
echo "  integer, intent(out) :: newinfo"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Info_free'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(info, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: info"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Info_get'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(info, key, valuelen, value, flag&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: info"
echo "  character(len=*), intent(in) :: key"
echo "  integer, intent(in) :: valuelen"
echo "  character(len=*), intent(out) :: value"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Info_get_nkeys'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(info, nkeys, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: info"
echo "  integer, intent(out) :: nkeys"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Info_get_nthkey'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(info, n, key, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: info"
echo "  integer, intent(in) :: n"
echo "  character(len=*), intent(out) :: key"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Info_get_valuelen'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(info, key, valuelen, flag, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: info"
echo "  character(len=*), intent(in) :: key"
echo "  integer, intent(out) :: valuelen"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Info_set'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(info, key, value, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: info"
echo "  character(len=*), intent(in) :: key"
echo "  character(len=*), intent(in) :: value"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Init'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Init_thread'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(required, provided, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: required"
echo "  integer, intent(out) :: provided"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Initialized'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(flag, ierr)"
echo "  use mpi_kinds"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Intercomm_create'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(local_comm, local_leader, bridge_comm, remote_leader, tag, &
        newintercomm, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: local_comm"
echo "  integer, intent(in) :: local_leader"
echo "  integer, intent(in) :: bridge_comm"
echo "  integer, intent(in) :: remote_leader"
echo "  integer, intent(in) :: tag"
echo "  integer, intent(out) :: newintercomm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Intercomm_merge'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(intercomm, high, newintercomm, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: intercomm"
echo "  logical, intent(in) :: high"
echo "  integer, intent(out) :: newintercomm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Iprobe'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(source, tag, comm, flag, status&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: source"
echo "  integer, intent(in) :: tag"
echo "  integer, intent(in) :: comm"
echo "  logical, intent(out) :: flag"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Irecv'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Irsend'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Is_thread_main'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(flag, ierr)"
echo "  use mpi_kinds"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Isend'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Issend'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Keyval_create'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(copy_fn, delete_fn, keyval, extra_state, ierr)"
echo "  use mpi_kinds"
echo "  external :: copy_fn"
echo "  external :: delete_fn"
echo "  integer, intent(out) :: keyval"
echo "  integer, intent(in) :: extra_state"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Keyval_free'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(keyval, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: keyval"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Op_create'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(function, commute, op, ierr)"
echo "  use mpi_kinds"
echo "  external :: function"
echo "  logical, intent(in) :: commute"
echo "  integer, intent(out) :: op"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Op_free'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(op, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: op"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Pack'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(inbuf, incount, datatype, outbuf, outsize, &
        position, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: incount"
  echo "  integer, intent(in) :: datatype"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(out) :: outsize"
  echo "  integer, intent(inout) :: position"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(inbuf, incount, datatype, outbuf, outsize, &
        position, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: incount"
  echo "  integer, intent(in) :: datatype"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(out) :: outsize"
  echo "  integer, intent(inout) :: position"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(inbuf, incount, datatype, outbuf, outsize, &
        position, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: incount"
  echo "  integer, intent(in) :: datatype"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(out) :: outsize"
  echo "  integer, intent(inout) :: position"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(inbuf, incount, datatype, outbuf, outsize, &
        position, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: incount"
  echo "  integer, intent(in) :: datatype"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(out) :: outsize"
  echo "  integer, intent(inout) :: position"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(inbuf, incount, datatype, outbuf, outsize, &
        position, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: incount"
    echo "  integer, intent(in) :: datatype"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(out) :: outsize"
    echo "  integer, intent(inout) :: position"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(inbuf, incount, datatype, outbuf, outsize, &
        position, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: incount"
    echo "  integer, intent(in) :: datatype"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(out) :: outsize"
    echo "  integer, intent(inout) :: position"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(inbuf, incount, datatype, outbuf, outsize, &
        position, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: incount"
    echo "  integer, intent(in) :: datatype"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(out) :: outsize"
    echo "  integer, intent(inout) :: position"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(inbuf, incount, datatype, outbuf, outsize, &
        position, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: incount"
    echo "  integer, intent(in) :: datatype"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(out) :: outsize"
    echo "  integer, intent(inout) :: position"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Pack_external'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(datarep, inbuf, incount, datatype, outbuf, &
        outsize, position, ierr)"
  echo "  use mpi_kinds"
  echo "  character(len=*), intent(in) :: datarep"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: incount"
  echo "  integer, intent(in) :: datatype"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: outbuf"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: outsize"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(datarep, inbuf, incount, datatype, outbuf, &
        outsize, position, ierr)"
  echo "  use mpi_kinds"
  echo "  character(len=*), intent(in) :: datarep"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: incount"
  echo "  integer, intent(in) :: datatype"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: outbuf"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: outsize"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(datarep, inbuf, incount, datatype, outbuf, &
        outsize, position, ierr)"
  echo "  use mpi_kinds"
  echo "  character(len=*), intent(in) :: datarep"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: incount"
  echo "  integer, intent(in) :: datatype"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: outbuf"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: outsize"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(datarep, inbuf, incount, datatype, outbuf, &
        outsize, position, ierr)"
  echo "  use mpi_kinds"
  echo "  character(len=*), intent(in) :: datarep"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: incount"
  echo "  integer, intent(in) :: datatype"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: outbuf"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: outsize"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(datarep, inbuf, incount, datatype, outbuf, &
        outsize, position, ierr)"
    echo "  use mpi_kinds"
    echo "  character(len=*), intent(in) :: datarep"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: incount"
    echo "  integer, intent(in) :: datatype"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: outsize"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(datarep, inbuf, incount, datatype, outbuf, &
        outsize, position, ierr)"
    echo "  use mpi_kinds"
    echo "  character(len=*), intent(in) :: datarep"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: incount"
    echo "  integer, intent(in) :: datatype"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: outsize"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(datarep, inbuf, incount, datatype, outbuf, &
        outsize, position, ierr)"
    echo "  use mpi_kinds"
    echo "  character(len=*), intent(in) :: datarep"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: incount"
    echo "  integer, intent(in) :: datatype"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: outsize"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(datarep, inbuf, incount, datatype, outbuf, &
        outsize, position, ierr)"
    echo "  use mpi_kinds"
    echo "  character(len=*), intent(in) :: datarep"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: incount"
    echo "  integer, intent(in) :: datatype"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: outsize"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Pack_external_size'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(datarep, incount, datatype, size, ierr)"
echo "  use mpi_kinds"
echo "  character(len=*), intent(in) :: datarep"
echo "  integer, intent(in) :: incount"
echo "  integer, intent(in) :: datatype"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: size"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Pack_size'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(incount, datatype, comm, size, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: incount"
echo "  integer, intent(in) :: datatype"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: size"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Pcontrol'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(level)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: level"

echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Probe'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(source, tag, comm, status, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: source"
echo "  integer, intent(in) :: tag"
echo "  integer, intent(in) :: comm"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Put'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: origin_addr"
  echo "  integer, intent(in) :: origin_count"
  echo "  integer, intent(in) :: origin_datatype"
  echo "  integer, intent(in) :: target_rank"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
  echo "  integer, intent(in) :: target_count"
  echo "  integer, intent(in) :: target_datatype"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(origin_addr, origin_count, origin_datatype, target_rank, target_disp, &
        target_count, target_datatype, win, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: origin_addr"
    echo "  integer, intent(in) :: origin_count"
    echo "  integer, intent(in) :: origin_datatype"
    echo "  integer, intent(in) :: target_rank"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: target_disp"
    echo "  integer, intent(in) :: target_count"
    echo "  integer, intent(in) :: target_datatype"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Query_thread'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(provided, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(out) :: provided"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Recv'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Recv_init'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, source, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Reduce'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Reduce_scatter'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, recvcounts, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcounts"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, recvcounts, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcounts"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, recvcounts, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcounts"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, recvcounts, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcounts"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, recvcounts, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcounts"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, recvcounts, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcounts"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, recvcounts, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcounts"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, recvcounts, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcounts"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

procedure='MPI_Register_datarep'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(datarep, read_conversion_fn, write_conversion_fn, dtype_file_extent_fn, extra_state&
        , ierr)"
echo "  use mpi_kinds"
echo "  character(len=*), intent(in) :: datarep"
echo "  external :: read_conversion_fn"
echo "  external :: write_conversion_fn"
echo "  external :: dtype_file_extent_fn"
echo "  integer, intent(in) :: extra_state"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Request_free'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(request, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: request"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Request_get_status'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(request, flag, status, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: request"
echo "  logical, intent(out) :: flag"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Rsend'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(ibuf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: ibuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(ibuf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: ibuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(ibuf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: ibuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(ibuf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: ibuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(ibuf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: ibuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(ibuf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: ibuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(ibuf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: ibuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(ibuf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: ibuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Rsend_init'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Scan'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: op"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, recvbuf, count, datatype, op, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: op"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Scatter'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, recvbuf, recvcount, &
        recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

# JMS see comment in ompi/mpi/f90/Makefile.am about WANT_2BUFFER_FUNCTIONS
if test "$WANT_2BUFFER_FUNCTIONS" = "1"; then

procedure='MPI_Scatterv'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcounts"
  echo "  integer, intent(in) :: displs"
  echo "  integer, intent(in) :: sendtype"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcounts"
  echo "  integer, intent(in) :: displs"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcounts"
  echo "  integer, intent(in) :: displs"
  echo "  integer, intent(in) :: sendtype"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcounts"
  echo "  integer, intent(in) :: displs"
  echo "  integer, intent(in) :: sendtype"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: root"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcounts"
    echo "  integer, intent(in) :: displs"
    echo "  integer, intent(in) :: sendtype"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcounts"
    echo "  integer, intent(in) :: displs"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcounts"
    echo "  integer, intent(in) :: displs"
    echo "  integer, intent(in) :: sendtype"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, sendcounts, displs, sendtype, recvbuf, &
        recvcount, recvtype, root, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcounts"
    echo "  integer, intent(in) :: displs"
    echo "  integer, intent(in) :: sendtype"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: root"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo

fi

procedure='MPI_Send'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Send_init'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Sendrecv'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, dest, sendtag, &
        recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: sendtag"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: recvtag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, dest, sendtag, &
        recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: sendtag"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: recvtag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, dest, sendtag, &
        recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: sendtag"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: recvtag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(sendbuf, sendcount, sendtype, dest, sendtag, &
        recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: sendbuf"
  echo "  integer, intent(in) :: sendcount"
  echo "  integer, intent(in) :: sendtype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: sendtag"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: recvbuf"
  echo "  integer, intent(in) :: recvcount"
  echo "  integer, intent(in) :: recvtype"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: recvtag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, dest, sendtag, &
        recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: sendtag"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: recvtag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, dest, sendtag, &
        recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: sendtag"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: recvtag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, dest, sendtag, &
        recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: sendtag"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: recvtag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(sendbuf, sendcount, sendtype, dest, sendtag, &
        recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: sendbuf"
    echo "  integer, intent(in) :: sendcount"
    echo "  integer, intent(in) :: sendtype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: sendtag"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: recvbuf"
    echo "  integer, intent(in) :: recvcount"
    echo "  integer, intent(in) :: recvtype"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: recvtag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Sendrecv_replace'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, sendtag, &
        source, recvtag, comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(inout) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: sendtag"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: recvtag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, sendtag, &
        source, recvtag, comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(inout) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: sendtag"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: recvtag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, sendtag, &
        source, recvtag, comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(inout) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: sendtag"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: recvtag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, sendtag, &
        source, recvtag, comm, status, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(inout) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: sendtag"
  echo "  integer, intent(in) :: source"
  echo "  integer, intent(in) :: recvtag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, sendtag, &
        source, recvtag, comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(inout) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: sendtag"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: recvtag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, sendtag, &
        source, recvtag, comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(inout) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: sendtag"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: recvtag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, sendtag, &
        source, recvtag, comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(inout) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: sendtag"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: recvtag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, sendtag, &
        source, recvtag, comm, status, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(inout) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: sendtag"
    echo "  integer, intent(in) :: source"
    echo "  integer, intent(in) :: recvtag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_SIZEOF'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(x, size, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: x"
  echo "  integer, intent(out) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(x, size, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: x"
  echo "  integer, intent(out) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(x, size, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: x"
  echo "  integer, intent(out) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(x, size, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: x"
  echo "  integer, intent(out) :: size"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(x, size, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: x"
    echo "  integer, intent(out) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(x, size, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: x"
    echo "  integer, intent(out) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(x, size, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: x"
    echo "  integer, intent(out) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(x, size, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: x"
    echo "  integer, intent(out) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Ssend'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Ssend_init'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: buf"
  echo "  integer, intent(in) :: count"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: dest"
  echo "  integer, intent(in) :: tag"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: request"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(buf, count, datatype, dest, tag, &
        comm, request, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: dest"
    echo "  integer, intent(in) :: tag"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Start'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(request, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: request"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Startall'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, array_of_requests, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, dimension(*), intent(inout) :: array_of_requests"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Status_set_cancelled'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(status, flag, ierr)"
echo "  use mpi_kinds"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
echo "  logical, intent(in) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Status_set_elements'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(status, datatype, count, ierr)"
echo "  use mpi_kinds"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
echo "  integer, intent(in) :: datatype"
echo "  integer, intent(in) :: count"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Test'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(request, flag, status, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: request"
echo "  logical, intent(out) :: flag"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Test_cancelled'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(status, flag, ierr)"
echo "  use mpi_kinds"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(in) :: status"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Testall'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, array_of_requests, flag, array_of_statuses, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, dimension(*), intent(inout) :: array_of_requests"
echo "  logical, intent(out) :: flag"
echo "  integer, dimension(MPI_STATUS_SIZE,*), intent(inout) :: array_of_statuses"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Testany'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, array_of_requests, index, flag, status&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, dimension(*), intent(inout) :: array_of_requests"
echo "  integer, intent(out) :: index"
echo "  logical, intent(out) :: flag"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Testsome'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(incount, array_of_requests, outcount, array_of_indices, array_of_statuses&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: incount"
echo "  integer, dimension(*), intent(inout) :: array_of_requests"
echo "  integer, intent(out) :: outcount"
echo "  integer, dimension(*), intent(out) :: array_of_indices"
echo "  integer, dimension(MPI_STATUS_SIZE,*), intent(inout) :: array_of_statuses"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Topo_test'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, status, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: status"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_commit'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: type"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_contiguous'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, oldtype, newtype, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, intent(in) :: oldtype"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_create_darray'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(size, rank, ndims, gsize_array, distrib_array, &
        darg_array, psize_array, order, oldtype, newtype, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: size"
echo "  integer, intent(in) :: rank"
echo "  integer, intent(in) :: ndims"
echo "  integer, dimension(*), intent(in) :: gsize_array"
echo "  integer, dimension(*), intent(in) :: distrib_array"
echo "  integer, dimension(*), intent(in) :: darg_array"
echo "  integer, dimension(*), intent(in) :: psize_array"
echo "  integer, intent(in) :: order"
echo "  integer, intent(in) :: oldtype"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_create_f90_complex'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(p, r, newtype, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: p"
echo "  integer, intent(in) :: r"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_create_f90_integer'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(r, newtype, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: r"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_create_f90_real'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(p, r, newtype, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: p"
echo "  integer, intent(in) :: r"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_create_hindexed'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, array_of_blocklengths, array_of_displacements, oldtype, newtype&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, dimension(*), intent(in) :: array_of_blocklengths"
echo "  integer(kind=MPI_ADDRESS_KIND), dimension(*), intent(in) :: array_of_displacements"
echo "  integer, intent(in) :: oldtype"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_create_hvector'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, blocklength, stride, oldtype, newtype&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, intent(in) :: blocklength"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: stride"
echo "  integer, intent(in) :: oldtype"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_create_indexed_block'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, blocklength, array_of_displacements, oldtype, newtype&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, intent(in) :: blocklength"
echo "  integer, dimension(*), intent(in) :: array_of_displacements"
echo "  integer, intent(in) :: oldtype"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_create_keyval'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type_copy_attr_fn, type_delete_attr_fn, type_keyval, extra_state, ierr)"
echo "  use mpi_kinds"
echo "  external :: type_copy_attr_fn"
echo "  external :: type_delete_attr_fn"
echo "  integer, intent(out) :: type_keyval"
echo "  integer, intent(in) :: extra_state"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_create_resized'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(oldtype, lb, extent, newtype, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: oldtype"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: lb"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: extent"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_create_struct'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, array_of_block_lengths, array_of_displacements, array_of_types, newtype&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, dimension(*), intent(in) :: array_of_block_lengths"
echo "  integer(kind=MPI_ADDRESS_KIND), dimension(*), intent(in) :: array_of_displacements"
echo "  integer, dimension(*), intent(in) :: array_of_types"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_create_subarray'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(ndims, size_array, subsize_array, start_array, order, &
        oldtype, newtype, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: ndims"
echo "  integer, dimension(*), intent(in) :: size_array"
echo "  integer, dimension(*), intent(in) :: subsize_array"
echo "  integer, dimension(*), intent(in) :: start_array"
echo "  integer, intent(in) :: order"
echo "  integer, intent(in) :: oldtype"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_delete_attr'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, type_keyval, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: type"
echo "  integer, intent(in) :: type_keyval"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_dup'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, newtype, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: type"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_extent'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, extent, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: type"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: extent"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_free'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: type"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_free_keyval'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type_keyval, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: type_keyval"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_get_attr'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, type_keyval, attribute_val, flag, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: type"
echo "  integer, intent(in) :: type_keyval"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: attribute_val"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_get_contents'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(mtype, max_integers, max_addresses, max_datatypes, array_of_integers, &
        array_of_addresses, array_of_datatypes, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: mtype"
echo "  integer, intent(in) :: max_integers"
echo "  integer, intent(in) :: max_addresses"
echo "  integer, intent(in) :: max_datatypes"
echo "  integer, dimension(*), intent(out) :: array_of_integers"
echo "  integer(kind=MPI_ADDRESS_KIND), dimension(*), intent(out) :: array_of_addresses"
echo "  integer, dimension(*), intent(out) :: array_of_datatypes"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_get_envelope'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, num_integers, num_addresses, num_datatypes, combiner&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: type"
echo "  integer, intent(out) :: num_integers"
echo "  integer, intent(out) :: num_addresses"
echo "  integer, intent(out) :: num_datatypes"
echo "  integer, intent(out) :: combiner"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_get_extent'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, lb, extent, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: type"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: lb"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: extent"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_get_name'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, type_name, resultlen, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: type"
echo "  character(len=*), intent(out) :: type_name"
echo "  integer, intent(out) :: resultlen"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_get_true_extent'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(datatype, true_lb, true_extent, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: datatype"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: true_lb"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: true_extent"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_hindexed'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, array_of_blocklengths, array_of_displacements, oldtype, newtype&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, dimension(*), intent(in) :: array_of_blocklengths"
echo "  integer(kind=MPI_ADDRESS_KIND), dimension(*), intent(in) :: array_of_displacements"
echo "  integer, intent(in) :: oldtype"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_hvector'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, blocklength, stride, oldtype, newtype&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, intent(in) :: blocklength"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: stride"
echo "  integer, intent(in) :: oldtype"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_indexed'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, array_of_blocklengths, array_of_displacements, oldtype, newtype&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, dimension(*), intent(in) :: array_of_blocklengths"
echo "  integer, dimension(*), intent(in) :: array_of_displacements"
echo "  integer, intent(in) :: oldtype"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_lb'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, lb, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: type"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: lb"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_match_size'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(typeclass, size, type, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: typeclass"
echo "  integer, intent(in) :: size"
echo "  integer, intent(out) :: type"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_set_attr'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, type_keyval, attr_val, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: type"
echo "  integer, intent(in) :: type_keyval"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: attr_val"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_set_name'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, type_name, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: type"
echo "  character(len=*), intent(in) :: type_name"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_size'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(type, size, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: type"
echo "  integer, intent(out) :: size"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_struct'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, array_of_blocklengths, array_of_displacements, array_of_types, newtype&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, dimension(*), intent(in) :: array_of_blocklengths"
echo "  integer(kind=MPI_ADDRESS_KIND), dimension(*), intent(in) :: array_of_displacements"
echo "  integer, dimension(*), intent(in) :: array_of_types"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_ub'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(mtype, ub, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: mtype"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: ub"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Type_vector'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, blocklength, stride, oldtype, newtype&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, intent(in) :: blocklength"
echo "  integer, intent(in) :: stride"
echo "  integer, intent(in) :: oldtype"
echo "  integer, intent(out) :: newtype"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Unpack'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(inbuf, insize, position, outbuf, outcount, &
        datatype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: insize"
  echo "  integer, intent(inout) :: position"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(in) :: outcount"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(inbuf, insize, position, outbuf, outcount, &
        datatype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: insize"
  echo "  integer, intent(inout) :: position"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(in) :: outcount"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(inbuf, insize, position, outbuf, outcount, &
        datatype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: insize"
  echo "  integer, intent(inout) :: position"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(in) :: outcount"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(inbuf, insize, position, outbuf, outcount, &
        datatype, comm, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: inbuf"
  echo "  integer, intent(in) :: insize"
  echo "  integer, intent(inout) :: position"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(in) :: outcount"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(inbuf, insize, position, outbuf, outcount, &
        datatype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: insize"
    echo "  integer, intent(inout) :: position"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(in) :: outcount"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(inbuf, insize, position, outbuf, outcount, &
        datatype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: insize"
    echo "  integer, intent(inout) :: position"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(in) :: outcount"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(inbuf, insize, position, outbuf, outcount, &
        datatype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: insize"
    echo "  integer, intent(inout) :: position"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(in) :: outcount"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(inbuf, insize, position, outbuf, outcount, &
        datatype, comm, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer, intent(in) :: insize"
    echo "  integer, intent(inout) :: position"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(in) :: outcount"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Unpack_external'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(datarep, inbuf, insize, position, outbuf, &
        outcount, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  character(len=*), intent(in) :: datarep"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: inbuf"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: insize"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(in) :: outcount"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(datarep, inbuf, insize, position, outbuf, &
        outcount, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  character(len=*), intent(in) :: datarep"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: inbuf"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: insize"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(in) :: outcount"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(datarep, inbuf, insize, position, outbuf, &
        outcount, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  character(len=*), intent(in) :: datarep"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: inbuf"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: insize"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(in) :: outcount"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(datarep, inbuf, insize, position, outbuf, &
        outcount, datatype, ierr)"
  echo "  use mpi_kinds"
  echo "  character(len=*), intent(in) :: datarep"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: inbuf"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: insize"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(out) :: outbuf"
  echo "  integer, intent(in) :: outcount"
  echo "  integer, intent(in) :: datatype"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(datarep, inbuf, insize, position, outbuf, &
        outcount, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  character(len=*), intent(in) :: datarep"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: insize"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(in) :: outcount"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(datarep, inbuf, insize, position, outbuf, &
        outcount, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  character(len=*), intent(in) :: datarep"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: insize"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(in) :: outcount"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(datarep, inbuf, insize, position, outbuf, &
        outcount, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  character(len=*), intent(in) :: datarep"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: insize"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(in) :: outcount"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(datarep, inbuf, insize, position, outbuf, &
        outcount, datatype, ierr)"
    echo "  use mpi_kinds"
    echo "  character(len=*), intent(in) :: datarep"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: inbuf"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: insize"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(inout) :: position"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(out) :: outbuf"
    echo "  integer, intent(in) :: outcount"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Wait'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(request, status, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: request"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Waitall'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, array_of_requests, array_of_statuses, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, dimension(*), intent(inout) :: array_of_requests"
echo "  integer, dimension(MPI_STATUS_SIZE,*), intent(inout) :: array_of_statuses"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Waitany'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, array_of_requests, index, status, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  integer, dimension(*), intent(inout) :: array_of_requests"
echo "  integer, intent(out) :: index"
echo "  integer, dimension(MPI_STATUS_SIZE), intent(inout) :: status"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Waitsome'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(incount, array_of_requests, outcount, array_of_indices, array_of_statuses&
        , ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: incount"
echo "  integer, dimension(*), intent(inout) :: array_of_requests"
echo "  integer, intent(out) :: outcount"
echo "  integer, dimension(*), intent(out) :: array_of_indices"
echo "  integer, dimension(MPI_STATUS_SIZE,*), intent(inout) :: array_of_statuses"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_call_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, errorcode, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: win"
echo "  integer, intent(in) :: errorcode"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_complete'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: win"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_create'

echo "interface ${procedure}"
echo
rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(base, size, disp_unit, info, comm, &
        win, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(in) :: base"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: size"
  echo "  integer, intent(in) :: disp_unit"
  echo "  integer, intent(in) :: info"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ikinds
do
  proc="${procedure}${rank}DI${kind}"
  echo "subroutine ${proc}(base, size, disp_unit, info, comm, &
        win, ierr)"
  echo "  use mpi_kinds"
  echo "  integer(kind=MPI_INTEGER${kind}_KIND), intent(in) :: base"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: size"
  echo "  integer, intent(in) :: disp_unit"
  echo "  integer, intent(in) :: info"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $rkinds
do
  proc="${procedure}${rank}DR${kind}"
  echo "subroutine ${proc}(base, size, disp_unit, info, comm, &
        win, ierr)"
  echo "  use mpi_kinds"
  echo "  real(kind=MPI_REAL${kind}_KIND), intent(in) :: base"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: size"
  echo "  integer, intent(in) :: disp_unit"
  echo "  integer, intent(in) :: info"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done

rank=0
for kind in $ckinds
do
  proc="${procedure}${rank}DC${kind}"
  echo "subroutine ${proc}(base, size, disp_unit, info, comm, &
        win, ierr)"
  echo "  use mpi_kinds"
  echo "  complex(kind=MPI_REAL${kind}_KIND), intent(in) :: base"
  echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: size"
  echo "  integer, intent(in) :: disp_unit"
  echo "  integer, intent(in) :: info"
  echo "  integer, intent(in) :: comm"
  echo "  integer, intent(in) :: win"
  echo "  integer, intent(out) :: ierr"
  echo "end subroutine ${proc}"
  echo
done


for rank in $array_ranks
do
  make_dims $rank
  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(base, size, disp_unit, info, comm, &
        win, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: base"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: size"
    echo "  integer, intent(in) :: disp_unit"
    echo "  integer, intent(in) :: info"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ikinds
  do
    proc="${procedure}${rank}DI${kind}"
    echo "subroutine ${proc}(base, size, disp_unit, info, comm, &
        win, ierr)"
    echo "  use mpi_kinds"
    echo "  integer(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(in) :: base"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: size"
    echo "  integer, intent(in) :: disp_unit"
    echo "  integer, intent(in) :: info"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $rkinds
  do
    proc="${procedure}${rank}DR${kind}"
    echo "subroutine ${proc}(base, size, disp_unit, info, comm, &
        win, ierr)"
    echo "  use mpi_kinds"
    echo "  real(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: base"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: size"
    echo "  integer, intent(in) :: disp_unit"
    echo "  integer, intent(in) :: info"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done

  for kind in $ckinds
  do
    proc="${procedure}${rank}DC${kind}"
    echo "subroutine ${proc}(base, size, disp_unit, info, comm, &
        win, ierr)"
    echo "  use mpi_kinds"
    echo "  complex(kind=MPI_REAL${kind}_KIND), dimension(${dim}), intent(in) :: base"
    echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: size"
    echo "  integer, intent(in) :: disp_unit"
    echo "  integer, intent(in) :: info"
    echo "  integer, intent(in) :: comm"
    echo "  integer, intent(in) :: win"
    echo "  integer, intent(out) :: ierr"
    echo "end subroutine ${proc}"
    echo
  done
  echo
done
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_create_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(function, errhandler, ierr)"
echo "  use mpi_kinds"
echo "  external :: function"
echo "  integer, intent(out) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_create_keyval'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win_copy_attr_fn, win_delete_attr_fn, win_keyval, extra_state, ierr)"
echo "  use mpi_kinds"
echo "  external :: win_copy_attr_fn"
echo "  external :: win_delete_attr_fn"
echo "  integer, intent(out) :: win_keyval"
echo "  integer, intent(in) :: extra_state"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_delete_attr'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, win_keyval, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: win"
echo "  integer, intent(in) :: win_keyval"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_fence'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(assert, win, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: assert"
echo "  integer, intent(in) :: win"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_free'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: win"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_free_keyval'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win_keyval, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: win_keyval"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_get_attr'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, win_keyval, attribute_val, flag, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: win"
echo "  integer, intent(in) :: win_keyval"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(out) :: attribute_val"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_get_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, errhandler, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: win"
echo "  integer, intent(out) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_get_group'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, group, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: win"
echo "  integer, intent(out) :: group"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_get_name'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, win_name, resultlen, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: win"
echo "  character(len=*), intent(out) :: win_name"
echo "  integer, intent(out) :: resultlen"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_lock'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(lock_type, rank, assert, win, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: lock_type"
echo "  integer, intent(in) :: rank"
echo "  integer, intent(in) :: assert"
echo "  integer, intent(in) :: win"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_post'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group, assert, win, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group"
echo "  integer, intent(in) :: assert"
echo "  integer, intent(in) :: win"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_set_attr'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, win_keyval, attribute_val, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: win"
echo "  integer, intent(in) :: win_keyval"
echo "  integer(kind=MPI_ADDRESS_KIND), intent(in) :: attribute_val"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_set_errhandler'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, errhandler, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: win"
echo "  integer, intent(in) :: errhandler"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_set_name'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, win_name, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: win"
echo "  character(len=*), intent(in) :: win_name"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_start'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(group, assert, win, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: group"
echo "  integer, intent(in) :: assert"
echo "  integer, intent(in) :: win"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_test'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, flag, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: win"
echo "  logical, intent(out) :: flag"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_unlock'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(rank, win, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: rank"
echo "  integer, intent(in) :: win"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Win_wait'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(win, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: win"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Close_port'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(port_name, ierr)"
echo "  use mpi_kinds"
echo "  character(len=*), intent(in) :: port_name"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Lookup_name'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(service_name, info, port_name, ierr)"
echo "  use mpi_kinds"
echo "  character(len=*), intent(in) :: service_name"
echo "  integer, intent(in) :: info"
echo "  character(len=*), intent(out) :: port_name"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Open_port'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(info, port_name, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: info"
echo "  character(len=*), intent(out) :: port_name"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Publish_name'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(service_name, info, port_name, ierr)"
echo "  use mpi_kinds"
echo "  character(len=*), intent(in) :: service_name"
echo "  integer, intent(in) :: info"
echo "  character(len=*), intent(in) :: port_name"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Unpublish_name'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(service_name, info, port_name, ierr)"
echo "  use mpi_kinds"
echo "  character(len=*), intent(in) :: service_name"
echo "  integer, intent(in) :: info"
echo "  character(len=*), intent(in) :: port_name"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_disconnect'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(comm, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(inout) :: comm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_get_parent'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(parent, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(out) :: parent"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_join'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(fd, intercomm, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: fd"
echo "  integer, intent(out) :: intercomm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_accept'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(port_name, info, root, comm, newcomm&
        , ierr)"
echo "  use mpi_kinds"
echo "  character(len=*), intent(in) :: port_name"
echo "  integer, intent(in) :: info"
echo "  integer, intent(in) :: root"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: newcomm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_connect'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(port_name, info, root, comm, newcomm&
        , ierr)"
echo "  use mpi_kinds"
echo "  character(len=*), intent(in) :: port_name"
echo "  integer, intent(in) :: info"
echo "  integer, intent(in) :: root"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: newcomm"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_spawn'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(command, argv, maxprocs, info, root, &
        comm, intercomm, array_of_errcodes, ierr)"
echo "  use mpi_kinds"
echo "  character(len=*), intent(in) :: command"
echo "  character(len=*), dimension(*), intent(in) :: argv"
echo "  integer, intent(in) :: maxprocs"
echo "  integer, intent(in) :: info"
echo "  integer, intent(in) :: root"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: intercomm"
echo "  integer, dimension(*), intent(out) :: array_of_errcodes"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo


procedure='MPI_Comm_spawn_multiple'

echo "interface ${procedure}"
echo
proc="${procedure}"
echo "subroutine ${proc}(count, array_of_commands, array_of_argv, array_of_maxprocs, array_of_info, &
        root, comm, intercomm, array_of_errcodes, ierr)"
echo "  use mpi_kinds"
echo "  integer, intent(in) :: count"
echo "  character(len=*), dimension(*), intent(in) :: array_of_commands"
echo "  character(len=*), dimension(count,*), intent(in) :: array_of_argv"
echo "  integer, dimension(*), intent(in) :: array_of_maxprocs"
echo "  integer, dimension(*), intent(in) :: array_of_info"
echo "  integer, intent(in) :: root"
echo "  integer, intent(in) :: comm"
echo "  integer, intent(out) :: intercomm"
echo "  integer, dimension(*), intent(out) :: array_of_errcodes"
echo "  integer, intent(out) :: ierr"
echo "end subroutine ${proc}"
echo
echo "end interface ${procedure}"
echo
echo

echo "Finished generating Fortran 90 interface functions" >&2