\def\filename{Moffetttuthes} 
%% modification of garyttuthes class which I
%% received from Dr. Thompson in CE
%%  Moffetttuthes by Corey A. Moffet  (cmoffet@nwrc.ars.usda.gov)
%%
%%  Please use this file and modify if in any way you see fit.
%%   It is mostly a disorganized collection of hacks but it worked for 
%%   me.  
%%
\def\fileversion{1.1}
\def\filedate{2003/01/10}  %% european style year/day/month
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{class/Moffetttuthes}[\filedate\space\fileversion\space%%
       LaTeX document class. By Corey A. Moffet]

\LoadClass[oneside,12pt]{book}
%%\usepackage{times}
%%
%%  EDIT the pdftitle and pdfauthor file.  This should be changed
%%    so that these values can be passed in the preamble
%%
\usepackage[pdftitle = {Competition Among Blue Grama and Buffalograss
Ecotypes: Effects of Soil and Past Neighbor Interactions},
pdfauthor = {Corey A. Moffet},
dvipdfm]{hyperref}
\usepackage{graphicx}
\usepackage{textcomp}
\usepackage{longtable}
%% order matters longtable must come before caption2 if you want
%%  consistent caption styles in both tables and longtables 
\usepackage[hang, nooneline, longtable]{caption2}
\usepackage[abbr,dcu]{harvard}
\usepackage{lscape}
\usepackage{afterpage}

%%
%%  NOTE:  This prbably should be moved to the preamble
%%   it should be document dependent not class dependent
%%
\graphicspath{{./figures/}}

%% \bibliographystyle{achicago}
%% \bibliographystyle{moffet}

%% caption2 commands
\renewcommand{\captionlabeldelim}{.~}
\renewcommand{\captionfont}{\linespread{1}\normalsize}
\renewcommand{\arraystretch}{0.6}

%% general commands
\widowpenalty=10000
\clubpenalty=10000
\renewcommand{\thechapter}{\Roman{chapter}}
\renewcommand{\thesection}{\arabic{chapter}.\arabic{section}}
%% subsections and below don't need changing
\renewcommand{\thefigure}{\arabic{chapter}.\arabic{figure}}
\renewcommand{\thetable}{\arabic{chapter}.\arabic{table}}
\renewcommand{\theequation}{\arabic{chapter}.\arabic{equation}}
%% \renewcommand\@pnumwidth{4em}
\renewcommand{\frontmatter}{%%
   \if@openright
      \cleardoublepage
   \else
      \clearpage
   \fi
   \@mainmatterfalse
   \pagenumbering{roman}            %% Front Matter roman numerals
   \setcounter{page}{2}             %% Start on the second page
}

\def\@makechapterhead#1{%%
  \vspace*{-20 \p@}%%
  {\parindent \z@ \centering \normalfont
    \ifnum \c@secnumdepth >\m@ne
      \if@mainmatter
         \@chapapp\space \thechapter
        \par\nobreak
%%        \vskip 20\p@
      \fi
    \fi
    \interlinepenalty\@M
     #1 \par\nobreak
    \vskip 20\p@
  }}

\def\@makeschapterhead#1{%%
  \vspace*{-20 \p@}%%
  {\parindent \z@ \centering
    \normalfont
    \interlinepenalty\@M
     #1\par\nobreak
    \vskip 20\p@
  }}

\renewcommand*\l@part{\@dottedtocline{0}{0em}{1.5em}}
\renewcommand*\l@chapter{\@dottedtocline{0}{0em}{1.5em}}
\renewcommand\contentsname{TABLE OF CONTENTS}
\renewcommand\listfigurename{LIST OF FIGURES}
\renewcommand\listtablename{LIST OF TABLES}
\renewcommand\bibname{LITERATURE CITED}
\renewcommand\indexname{INDEX}
\renewcommand\figurename{Figure}
\renewcommand\tablename{Table}
\renewcommand\partname{PART}
\renewcommand\chaptername{CHAPTER}
\renewcommand\appendixname{APPENDIX}
\renewcommand\harvardand{and}

%%%%%%%%%%%%% ADDED FROM BARBI DICKENSHEET'S FILE JUNE 2003 %%%%%%%%%%%%%%%%
\def\@chapter[#1]#2{\ifnum \c@secnumdepth >\m@ne
                       \if@mainmatter
                         \refstepcounter{chapter}%
                         \typeout{\@chapapp\space\thechapter.}%
                         \addcontentsline{toc}{chapter}%
                         %RIGHT HERE is where it differs from book.cls
                                   {\protect\numberline{\thechapter .}#1}%
                       \else
                         \addcontentsline{toc}{chapter}{#1}%
                       \fi
                    \else
                      \addcontentsline{toc}{chapter}{#1}%
                    \fi
                    \chaptermark{#1}%
                    %% \addtocontents{lof}{\protect\addvspace{10\p@}}%
                    %% \addtocontents{lot}{\protect\addvspace{10\p@}}%
                    \if@twocolumn
                      \@topnewpage[\@makechapterhead{#2}]%
                    \else
                      \@makechapterhead{#2}%
                      \@afterheading
                    \fi}

\renewcommand\listoffigures{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\listfigurename
      \@mkboth{\MakeUppercase\listfigurename}%
              {\MakeUppercase\listfigurename}}%
           \addcontentsline{toc}{chapter}{\listfigurename}
    \@starttoc{lof}%
    \if@restonecol\twocolumn\fi
    }
\renewcommand\listoftables{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\listtablename
      \@mkboth{%
          \MakeUppercase\listtablename}{\MakeUppercase\listtablename}}%
    \addcontentsline{toc}{chapter}{\listtablename}
    \@starttoc{lot}%
    \if@restonecol\twocolumn\fi
    }

\renewcommand\chapter{\if@openright\cleardoublepage\else\clearpage\fi
                    \thispagestyle{plain}%
                    \global\@topnum\z@
                    \@afterindenttrue
                    \secdef\@chapter\@schapter
                    }

\renewcommand\section{\@startsection {section}{1}{\z@}%
                                   {3.5ex \@plus 1ex \@minus .2ex}%
                                   {.3ex \@plus.2ex}%
                                   {\reset@font\normalsize\mdseries\centering}}
\renewcommand\subsection{\@startsection{subsection}{2}{\z@}%
                                     {3.25ex\@plus 1ex \@minus .2ex}%
                                     {.2ex \@plus .2ex}%
                                     {\reset@font\normalsize
                                     \raggedright\noindent\mdseries\underline}}
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
%%%%%%%%%%%%%  END ADDITION  %%%%%%%%%%%%%%%%%%%%%%%%%%


\setlength{\oddsidemargin}{.5in}
\setlength{\evensidemargin}{.5in}
\setlength{\textwidth}{6in}
\setlength{\topmargin}{-.5in}
\setlength{\textheight}{8.65in}
\renewcommand\floatpagefraction{.9}
\renewcommand\textfraction{.1}
\setcounter{secnumdepth}{3}
\setcounter{tocdepth}{3}
\pagestyle{plain}
\renewcommand{\baselinestretch}{1.5}
\LTcapwidth=\textheight


%% My definitions
\makeatletter
\newcommand\centrulefill{\leavevmode
 \begingroup %% local length assignments!
   \setlength{\dimen@}{0.5ex}%%  or whatever positioning
   \addtolength{\dimen@}{0.4pt}%%  or whatever thickness
   \leaders\hrule height\dimen@ depth -.5ex \hfill
 \endgroup
 \kern\z@}

\newenvironment{singlespace}{%%
    \small\renewcommand{\baselinestretch}{1}\normalsize}
    {\small\normalsize}


\let\oldquote\quote
\let\endoldquote\endquote
\renewenvironment{quote}{\begin{oldquote}\begin{singlespace}}%%
  {\end{singlespace}\end{oldquote}}

\newcommand\degree{\textdegree}
\newcommand\degreeC{\textcelsius}

%%
%%  Some Munsell soil color names 
%%
\newcommand{\TwoFiveYThreeOne}{very dark gray (2.5Y 3/1)}
\newcommand{\TwoFiveYFourOne}{dark gray (2.5Y 4/1)}
\newcommand{\TwoFiveYFourTwo}{dark grayish brown (2.5Y 4/2)}
\newcommand{\TwoFiveYSixTwo}{light brownish gray (2.5Y 6/2)}
\newcommand{\TenYRThreeOne}{very dark gray (10YR 3/1)}
\newcommand{\TenYRThreeTwo}{very dark grayish brown (10YR 3/2)}
\newcommand{\TenYRFourTwo}{dark grayish brown (10YR 4/2)}
\newcommand{\SevenFiveYRThreeTwo}{dark brown (7.5YR 3/2)}
\newcommand{\SevenFiveYRThreeThree}{dark brown (7.5YR 3/3)}
\newcommand{\SevenFiveYRThreeFour}{dark brown (7.5YR 3/4)}
\newcommand{\SevenFiveYRFourFour}{brown (7.5YR 4/4)}
\newcommand{\SevenFiveYRFiveSix}{strong brown (7.5YR 5/6)}
\newcommand{\SevenFiveYRSixSix}{reddish yellow (7.5YR 6/6)}
\newcommand{\SevenFiveYRSevenSix}{reddish yellow (7.5YR 7/6)}
\newcommand{\SevenFiveYREightThree}{pink (7.5YR 8/3)}
\newcommand{\SevenFiveYREightFour}{pink (7.5YR 8/4)}
\newcommand{\SevenFiveYREightSix}{pink (7.5YR 8/6)}
\newcommand{\FiveYRFourFour}{reddish brown (5YR 4/4)}
\newcommand{\FiveYRFourSix}{yellowish red (5YR 4/6)}
\newcommand{\FiveYRFiveSix}{yellowish red (5YR 5/6)}
\newcommand{\FiveYRSixSix}{reddish yellow (5YR 6/6)}
%%
%%  MISC
%%
\newcommand{\LatLong}[6]{#1\textdegree\ #2' #3''~N, #4\textdegree\ #5' 
  #6''~W}
\newcommand{\buda}[1]{{\scshape buda}$_{\mbox{#1}}$}
\newcommand{\bogr}[1]{{\scshape bogr}$_{\mbox{#1}}$}
\newcommand{\alphalevel}[1]{$\alpha = #1$}
\newcommand{\dif}{\mathrm{d}}
