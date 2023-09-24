using System.CommandLine;
using System.CommandLine.Builder;
using System.CommandLine.Parsing;
using iText.Kernel.Pdf;
using iText.Kernel.Utils;

namespace AppendPdf
{
    class Program
    {
        public static void PdfAppender(string filePath, string[] PDFs) 
        {
            PdfDocument newPdf = new PdfDocument(new PdfWriter(filePath));
            PdfMerger merger = new PdfMerger(newPdf);


            for (int x = 0; x < PDFs.Length; x++)
            {
                PdfDocument PDF = new PdfDocument(new PdfReader(PDFs[x]));
                merger.Merge(PDF, 1, PDF.GetNumberOfPages());

                PDF.Close();
            }

            newPdf.Close();
        }
        public static string[] ParseString(string delimited_str)
        {
            // split on semicolons
            string[] parsedString = delimited_str.Split(';');

            // trim down leading and trailing whitespaces
            for (int i = 0; i < parsedString.Length; i++)
            {
                parsedString[i] = parsedString[i].Trim();
            }
            return parsedString;
        }
        public static async Task<int> Main(string[] args) 
        {
            /* ARGS:
             * -f, --filePath
             * -pdfs 
             */

            var filePath_option = new Option<string>(
                new string[] { "-path", "--filePath" },
                "New PDF file path");

            var PDFs_option = new Option<string>(
                new string[] { "-pdfs" },
                "PDF paths");

            var rootCommand = new RootCommand("Appends PDF files together given list of PDF paths")
            {
                filePath_option,
                PDFs_option
            };

            rootCommand.SetHandler(
                (filePath_value, PDFs_value) =>
                {
                    OnHandle(filePath_value, PDFs_value);
                },
                filePath_option, PDFs_option);

            var commandLineBuilder = new CommandLineBuilder(rootCommand)
                .UseDefaults();
            var parser = commandLineBuilder.Build();
            return await parser.InvokeAsync(args).ConfigureAwait(false);
        }

        private static void OnHandle(string filePath, string PDFs_value)
        {
            if (string.IsNullOrEmpty(filePath)) 
            {
                throw new ArgumentNullException(
                    "--filePath is required; please ensure that a value is provided");
            }
            if (string.IsNullOrEmpty(PDFs_value))
            {
                throw new ArgumentNullException(
                    "-pdfs is required; please ensure that a value is provided");
            }

            PdfAppender(filePath, ParseString(PDFs_value));
        }
    }
}