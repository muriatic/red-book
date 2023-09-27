# Red Book v2.0.0 (C++ refactor)
Resource for Compiling Members' Professional Information into Single PDF (Red Book)

## Features:
- Major and Minor autocorrection using list of accepted majors/minors
- Graduation Date validation using RegEx
- Phone number validation using RegEx
- Email validation using RegEx
- LinkedIn validation using RegEx
- Current position autocorrection using list of accepted positions

## Missing Features:
- HTML compilation (needs to be reintroduced)
- Headshot processing (needs to be reintroduced)
- Resume Parsing (in progress)
- Config file

## Dependencies:
### Languages / Frameworks:
- C++ 20
- .NET 6.0 (found [here](https://dotnet.microsoft.com/en-us/download/dotnet/thank-you/sdk-6.0.412-windows-x64-installer))

In order to edit and compile the C# for which the project is dependent on, the following C# packages must be installed:
- itext7 v8.0.0
- itext7.bouncy-castle v8.0.0
- itext7.pdfhtml v5.0.0
- System.CommandLine Prerelease

## Setup:
### Required Executables:
- html-to-pdf.exe v2.0.0
- pdf-reader.exe v2.0.0
- append-pdfs.exe v1.0.0

### Required Files:
#### CSVs/majors.csv, CSVs/minors.csv
- Row 1 = ```majors``` (```minors```)
- Row 2+ = correct spelling of majors (minors)

#### CSVs/FALL2023_positions.csv
- Row 1 = [```Positions```, ```aka```]
- Row 2+ = [official position name, position aliases]

#### CSVs/responses.csv
- Row 1 = [```Name (First and Last)```, ```Professional Headshot```, ```Major(s)```, ```Minor(s)```, ```Expected Graduation (month year)```, ```Pronouns```, ```Phone Number```, ```Email```, ```LinkedIn```, ```Current AKPsi Position```, ```Attach Current Resume```]

### Recommended IDEs:
- Visual Studio Community (or better)
- Visual Studio Code (with [C# Dev Kit](https://marketplace.visualstudio.com/items?itemName=ms-dotnettools.csdevkit))

### C# Packages:
#### dotnet CLI in Visual Studio:
    Tools -> Command Line -> Developer Command Prompt

#### dotnet CLI in Visual Studio Code:
    View -> Terminal

#### iText7 Packages:
- dotnet add package itext7
- dotnet add package itext7.bouncy-castle-adapter
- dotnet add package itext7.pdfhtml

#### .NET CommandLine Package for C#:
- dotnet add package System.CommandLine --prerelease 

## Troubleshooting:
### C#:
- Issue with PDF Processing? -> ensure the versions of the C# packages are the same; if not, try the above ```dotnet add package``` commands with the version like ```--version x.x.x```.

### Finalized PDF:
- No formatting applied to compiled PDF from HTML? -> ensure absolute path to the JS, CSS files are used before the HTML is compiled.
- Images missing in compiled PDF but not HTML? -> ensure absolute path to image is provided; that there are **NO** spaces in the path, using ```_``` or like characters as needed.
- Rendered image has formatting but is not as intended? -> check the generated HTML found in HTMLs in a text editor for proper spacing and formatting; if this persists, the HTML or CSS element/style may be unsupported by iText7.

### Editing HTML Source Code:
- Changing the source HTML file for this project? -> you can change it in _config.cfg_ (not yet implemented) by replacing the ```'source2.html'``` in ```SOURCE_CODE``` with your new HTML file.
- HTML doesn't look quite right (specifically towards the bottom of the page)? -> use Chrome, not fullscreen, on a 16:9 1080p monitor, with 100% zoom.

## NOTES:
- Currently source2.html and stylesheet2.css is the foundation of the current Red Book so all changes to design should be applied to source2.html and stylesheet2.css OR change ```SOURCE_CODE``` in _config.cfg_ (not yet implemented).
- iText7 fails to support many modern (and older) HTML and CSS features, so many formatting issues may arise from lack of support and not erroneous code.
- iText7 can not handle spaces in file paths within HTML/CSS/JS code. 
