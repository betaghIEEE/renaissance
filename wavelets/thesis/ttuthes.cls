\def\filename{gilliam_cls}
\def\fileversion{1.0}
\def\filedate{1997/01/11}  % european style year/day/month
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{ttuthes}[\filedate\space\fileversion\space%
       LaTeX document class.]

\LoadClass[oneside,12pt]{book}


\renewcommand{\frontmatter}{%
   \if@openright
      \cleardoublepage
   \else
      \clearpage
   \fi
   \@mainmatterfalse
   \pagenumbering{roman}            %% Front Matter roman numerals
   \setcounter{page}{2}             %% Start on the second page
}


\def\@makechapterhead#1{%
  \vspace*{-20 \p@}%
  {\parindent \z@ \centering \normalfont
    \ifnum \c@secnumdepth >\m@ne
      \if@mainmatter
         \@chapapp\space \thechapter
        \par\nobreak
%        \vskip 20\p@
      \fi
    \fi
    \interlinepenalty\@M
     #1 \par\nobreak
    \vskip 20\p@
  }}

\def\@makeschapterhead#1{%
  \vspace*{-20 \p@}%
  {\parindent \z@ \centering
    \normalfont
    \interlinepenalty\@M
     #1\par\nobreak
    \vskip 20\p@
  }}




\renewcommand*\l@part{\@dottedtocline{0}{0em}{1.5em}}
%\renewcommand*\l@chapter{\@dottedtocline{0}{1.5em}{2.5em}}
\renewcommand*\l@chapter{\@dottedtocline{0}{1.5em}{2.75em}}
\renewcommand*\l@section{\@dottedtocline{1}{4em}{2.75em}}
\renewcommand*\l@subsection{\@dottedtocline{2}{5.75em}{2.75em}}

%\def\numberlinetwo#1{\advance\hangindent\@tempdima
%  \hbox to\@tempdima{\hfil #1\enspace}}
% The above Two lines in conjunction with the %*ed line below make
%   the table of contents have non-aligned entries for mainmatter
%   chapters.


\def\@chapter[#1]#2{\ifnum \c@secnumdepth >\m@ne
                       \if@mainmatter
                         \refstepcounter{chapter}%
                         \typeout{\@chapapp\space\thechapter.}%
                         \addcontentsline{toc}{chapter}%
                                   {\protect\numberline{\arabic{chapter}}#1}% 
         %*
                       \else
                         \addcontentsline{toc}{chapter}{#1}%
                       \fi
                    \else
                      \addcontentsline{toc}{chapter}{#1}%
                    \fi
                    \chaptermark{#1}%
                    \addtocontents{lof}{\protect\addvspace{10\p@}}%
                    \addtocontents{lot}{\protect\addvspace{10\p@}}%
                    \if@twocolumn
                      \@topnewpage[\@makechapterhead{#2}]%
                    \else
                      \@makechapterhead{#2}%
                      \@afterheading
                    \fi}

\renewcommand\contentsname{CONTENTS}
\renewcommand\listfigurename{LIST OF FIGURES}
\renewcommand\listtablename{LIST OF TABLES}
\renewcommand\bibname{BIBLIOGRAPHY}
\renewcommand\indexname{INDEX}
\renewcommand\figurename{Figure}
\renewcommand\tablename{Table}
\renewcommand\partname{PART}
\renewcommand\chaptername{CHAPTER}
\renewcommand\appendixname{APPENDIX}
\renewcommand\today{}

\renewcommand\chapter{\if@openright\cleardoublepage\else\clearpage\fi
                    \thispagestyle{plain}%
                    \global\@topnum\z@
                    \@afterindenttrue
                    \secdef\@chapter\@schapter}

\renewcommand\section{\@startsection {section}{1}{\z@}%
                                   {3.5ex \@plus 1ex \@minus .2ex}%
                                   {.3ex \@plus.2ex}%
                                   {\normalfont\normalsize\centering
                                   }}
\renewcommand\subsection{\@startsection{subsection}{2}{\z@}%
                                     {3.25ex\@plus 1ex \@minus .2ex}%
                                     {.2ex \@plus .2ex}%
                                     {\normalfont\normalsize
                                     \raggedright\noindent}}
\renewcommand\subsubsection{\@startsection{subsubsection}{3}{\z@}%
                                     {3.25ex\@plus 1ex \@minus .2ex}%
                                     {.2ex \@plus .2ex}%
                                     {\normalfont\normalsize
                                     \raggedright\noindent}}
\renewcommand\paragraph{\@startsection{paragraph}{4}{\z@}%
                                    {3.25ex \@plus1ex \@minus.2ex}%
                                    {-1em}%
                                    {\normalfont\normalsize}}
\renewcommand\subparagraph{\@startsection{subparagraph}{5}{\parindent}%
                                       {3.25ex \@plus1ex \@minus .2ex}%
                                       {-1em}%
                                      {\normalfont\normalsize}}

    \setlength{\oddsidemargin}{.45in}  %% These margins worked for my
    \setlength{\evensidemargin}{.5in}   % Dissertation
    \setlength{\textwidth}{6in}
    \setlength{\topmargin}{-.5in}
    \setlength{\textheight}{8.65in}
    \newcommand{\tr}{^\mathsf{T}}  %% puts the transpose symbol on a Matrix
    \newcommand{\bs}{\boldsymbol}  %% Shortcut for \boldsymbol
    \newcommand{\R}{\mathbb{R}}    %% The REAL symbol
    \newcommand{\ep}
               {\hfill\rule[-.23ex]{1.6ex}{1.6ex}\medskip} %% end of proof  symbol
    \renewcommand\floatpagefraction{.9}
    \renewcommand\textfraction{.1}
    \setcounter{secnumdepth}{3}
    \setcounter{tocdepth}{3}
    \pagestyle{plain}              %% For the most part this gives  pagenumbering
    \renewcommand{\baselinestretch}{1.5}  % like the grad school wants


\RequirePackage{epsfig,amsmath,amssymb,amsfonts}

\def\@begintheorem#1#2{\trivlist
   \item[\hskip \labelsep{#1\ #2.}]\itshape}


