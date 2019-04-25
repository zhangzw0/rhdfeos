
mydemofun <- function(fpath) {
  cAns <- .Call("Reos_demo", PACKAGE = "rhdfeos")
  return(cAns)
}
