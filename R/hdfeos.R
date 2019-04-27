he2_open <- function(filename, mode = "read", verbos = FALSE) {
  rv <- list()
  rv$fid <- -1
  if (mode == "read")
    rv$mode <- 1

  rv <- .C("r_he2_open",
           filename = as.character(filename),
           mode = as.integer(rv$mode),
           fid = as.integer(rv$fid), PACKAGE="rhdfeos")
  if( rv$fid == -1 )
    print("fid is -1, cannot open the file")

  class(rv) <- "he2"
  rv
}

he2_close <- function(x) {
  if (class(x) != "he2")
    stop("the paramter must be class he2")
  rv = .C("r_he2_close",
          as.integer(x$fid), PACKAGE="rhdfeos")
}


print.he2 <- function(x, ...) {
  print("he2")
}


he2_inqdims <- function(x, swathname) {
  if (class(x) != "he2") stop("the paramter must be class he2")
  rv = .C("r_he2_inqdims",
          as.integer(x$fid), as.character(swathname), PACKAGE="rhdfeos")
}

he2_inqdimmaps <- function(x, swathname) {
  if (class(x) != "he2") stop("the paramter must be class he2")
  rv = .C("r_he2_inqdimmaps",
          as.integer(x$fid), as.character(swathname), PACKAGE="rhdfeos")
}

he2_inqdatafields <- function(x, swathname) {
  if (class(x) != "he2") stop("the paramter must be class he2")
  rv = .C("r_he2_inqdatafields",
          as.integer(x$fid), as.character(swathname), PACKAGE="rhdfeos")
}

he2_inqgeolocfields <- function(x, swathname) {
  if (class(x) != "he2") stop("the paramter must be class he2")
  rv = .C("r_he2_inqgeolocfields",
          as.integer(x$fid), as.character(swathname), PACKAGE="rhdfeos")
}

he2_fieldinfo <- function(x, swathname, fieldname) {
  if (class(x) != "he2") stop("the paramter must be class he2")
  rv = .C("r_he2_fieldinfo",
          as.integer(x$fid), as.character(swathname), as.character(fieldname),
          PACKAGE="rhdfeos")
}
