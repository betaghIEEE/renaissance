#! /bin/sh

. "$1/fortran_kinds.sh"

procedure='MPI_Buffer_detach'

rank=0
for kind in $lkinds
do
  proc="${procedure}${rank}DL${kind}"
  echo "subroutine ${proc}(buffer, size, ierr)"
  echo "  use mpi_kinds"
  echo "  logical(kind=MPI_INTEGER${kind}_KIND), intent(out) :: buffer"
  echo "  integer, intent(out) :: size"
  echo "  integer, intent(out) :: ierr"
    echo "  call ${procedure}(buffer, size, ierr)"

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
    echo "  call ${procedure}(buffer, size, ierr)"

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
    echo "  call ${procedure}(buffer, size, ierr)"

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
    echo "  call ${procedure}(buffer, size, ierr)"

  echo "end subroutine ${proc}"
  echo
done


for rank in $ranks
do
  case "$rank" in  1)  dim=':'  ;  esac
  case "$rank" in  2)  dim=':,:'  ;  esac
  case "$rank" in  3)  dim=':,:,:'  ;  esac
  case "$rank" in  4)  dim=':,:,:,:'  ;  esac
  case "$rank" in  5)  dim=':,:,:,:,:'  ;  esac
  case "$rank" in  6)  dim=':,:,:,:,:,:'  ;  esac
  case "$rank" in  7)  dim=':,:,:,:,:,:,:'  ;  esac

  for kind in $lkinds
  do
    proc="${procedure}${rank}DL${kind}"
    echo "subroutine ${proc}(buffer, size, ierr)"
    echo "  use mpi_kinds"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buffer"
    echo "  integer, intent(out) :: size"
    echo "  integer, intent(out) :: ierr"
    echo "  call ${procedure}(buffer, size, ierr)"

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
    echo "  call ${procedure}(buffer, size, ierr)"

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
    echo "  call ${procedure}(buffer, size, ierr)"

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
    echo "  call ${procedure}(buffer, size, ierr)"

    echo "end subroutine ${proc}"
    echo
  done
done
echo
echo
