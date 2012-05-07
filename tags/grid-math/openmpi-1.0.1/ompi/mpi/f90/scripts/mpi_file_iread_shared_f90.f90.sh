#! /bin/sh

. "$1/fortran_kinds.sh"

procedure='MPI_File_iread_shared'

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
    echo "  call ${procedure}(fh, buf, count, datatype, request&
        , ierr)"

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
    echo "  call ${procedure}(fh, buf, count, datatype, request&
        , ierr)"

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
    echo "  call ${procedure}(fh, buf, count, datatype, request&
        , ierr)"

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
    echo "  call ${procedure}(fh, buf, count, datatype, request&
        , ierr)"

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
    echo "subroutine ${proc}(fh, buf, count, datatype, request&
        , ierr)"
    echo "  use mpi_kinds"
    echo "  integer, intent(inout) :: fh"
    echo "  logical(kind=MPI_INTEGER${kind}_KIND), dimension(${dim}), intent(out) :: buf"
    echo "  integer, intent(in) :: count"
    echo "  integer, intent(in) :: datatype"
    echo "  integer, intent(out) :: request"
    echo "  integer, intent(out) :: ierr"
    echo "  call ${procedure}(fh, buf, count, datatype, request&
        , ierr)"

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
    echo "  call ${procedure}(fh, buf, count, datatype, request&
        , ierr)"

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
    echo "  call ${procedure}(fh, buf, count, datatype, request&
        , ierr)"

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
    echo "  call ${procedure}(fh, buf, count, datatype, request&
        , ierr)"

    echo "end subroutine ${proc}"
    echo
  done
done
echo
echo
