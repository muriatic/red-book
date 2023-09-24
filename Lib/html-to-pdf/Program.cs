using iText.Html2pdf;
using iText.Kernel.Geom;
using iText.Kernel.Pdf;
using System.CommandLine;
using System.CommandLine.Builder;
using System.CommandLine.Parsing;


namespace ConvertHTML
{
    class Program
    {
        static int[] CreateChunks(int length, int n)
        {
            // create n number chunks
            int[] chunks = new int[n];

            int quotient = length / n;
            int remainder = length % n;

            // if its greater than one we can spread it out more
            if (remainder > 0)
            {
                for (int i = 0; i < n; i++)
                {
                    int addition = 0;

                    if (remainder > 0)
                    {
                        addition = 1;
                        remainder--;
                    }

                    chunks[i] = quotient + addition;
                }
            }
            else
            {
                for (int i = 0; i < n; i++)
                {
                    chunks[i] = quotient;
                }

            }

            return chunks;
        }
        static string[] CombinePaths(string DIRECTORY, string[] names, string file_type)
        {
            string[] paths = new string[names.Length];

            for (int i = 0; i < names.Length; i++)
            {
                string file_name = names[i] + file_type;
                string combined_path = System.IO.Path.Combine(DIRECTORY, file_name);
                paths[i] = combined_path;
            }

            return paths;
        }

        static void Converter(string HTMLS_FOLDER, string PDFS_FOLDER, string[] names, int threads)
        {
            int names_length = names.Length;
            int subprocesses; 

            // if no value of threads is inputted; default to 2
            if (threads == 0) 
            {
                subprocesses = 2;
            }
            else 
            {
                subprocesses = threads;
            }

            // change # of subprocess to the length of names if length of names is less than the number of subprocesses
            if (names_length < subprocesses)
            {
                subprocesses = names_length;
            }

            // create array into even(ish) # chunks based on # of subprocesses
            int[] chunks = CreateChunks(names_length, subprocesses);

            // create nested arrays with subprocesses # of top nests for HTML and PDF names
            string[][] chunked_HTML_names = new string[subprocesses][];
            string[][] chunked_PDF_names = new string[subprocesses][];

            int last_idx = 0;
            for (int i = 0; i < subprocesses; i++) // O(N^2) time complexity
            {
                // get chunk of names after last cutoff and this one
                var string_chunk = names[last_idx..(chunks[i] + last_idx)];

                string[] combined_HTML_paths = CombinePaths(HTMLS_FOLDER, string_chunk, ".html");
                string[] combined_PDF_paths = CombinePaths(PDFS_FOLDER, string_chunk, ".pdf");

                chunked_HTML_names[i] = combined_HTML_paths;
                chunked_PDF_names[i] = combined_PDF_paths;

                last_idx = chunks[i];
            }

            Parallel.For(0, subprocesses, i =>
            {
                NewThread(chunked_HTML_names[i], chunked_PDF_names[i]);
            });

            /*ThreadPool.SetMaxThreads(subprocesses, subprocesses);

            for (int i = 0; i < subprocesses ; i++)
            {

            }

            Thread thread0 = new Thread(() => NewThread(chunked_HTML_names[0], chunked_PDF_names[0]));
            Thread thread1 = new Thread(() => NewThread(chunked_HTML_names[1], chunked_PDF_names[1]));

            thread0.Start();
            thread1.Start();

            Thread[] threads = new Thread[subprocesses];
            for (int i = 0; i < subprocesses; i++)
            {
                Console.WriteLine("[{0}]", string.Join(", ", chunked_HTML_names[i]));
                threads[i] = new Thread(() => NewThread(chunked_HTML_names[i], chunked_PDF_names[i]));
            }

            // start the threads
            for (int i = 0; i < subprocesses; i++)
            {
                threads[i].Start();
            }*/
        }

        static void NewThread(string[] HTML_paths, string[] PDF_paths)
        {
            for (int i = 0; i < HTML_paths.Length; i++)
            {
                ConvertHTMLtoPDF(HTML_paths[i], PDF_paths[i]);
            }
        }
        static void ConvertHTMLtoPDF(string HTMLFilePath, string PdfFilePath)
        {
            // Note iText7 CANNOT HANDLE CSS CALCulations and Variables

            //check if the source file exists
            if (!File.Exists(HTMLFilePath))
            {
                throw new FileNotFoundException(
                    "The source file: [" + HTMLFilePath + "] does not exist, please check that your code is correct");
            }

            Console.WriteLine("Valid Arguments, Converting to PDF");

            PdfWriter writer = new PdfWriter(PdfFilePath);
            PdfDocument pdfDocument = new PdfDocument(writer);
            pdfDocument.SetDefaultPageSize(PageSize.LETTER);

            HtmlConverter.ConvertToPdf(new FileStream(HTMLFilePath, FileMode.Open), pdfDocument);

            pdfDocument.Close();
        }

        // could very well have to change this to see how Python will naturally send arrays to EXE, rather keep it inline with Python to avoid extra BS
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
             * HTML_FOLDER;
             * PDFS_FOLDER;
             * names {separated by ;}
             * threads (optional)
            */

            var HTMLS_FOLDER_option = new Option<string>(
                new string[] { "-htmls", "--html-folder" },
                "Where the HTML files are stored");

            var PDFS_FOLDER_option = new Option<string>(
                new string[] { "-pdfs", "--pdf-folder" },
                "Where the PDF files are stored");

            var names_option = new Option<string>(
                new string[] { "-names" },
                "List of names separated by a semi colon (e.g. John Doe; Jane Doe)");

            var threads_option = new Option<int>(
                new string[] { "-threads", "--subprocesses" },
                "Number of subprocesses or threads (default = 2)");

            var rootCommand = new RootCommand("Converts HTMLs to PDFs provided the names of the files")
            {
                HTMLS_FOLDER_option,
                PDFS_FOLDER_option,
                names_option,
                threads_option
            };

            rootCommand.SetHandler(
                (HTMLS_FOLDER_value, PDFS_FOLDER_value, names_value, threads_value) =>
                {
                    OnHandle(HTMLS_FOLDER_value, PDFS_FOLDER_value, names_value, threads_value);
                },
                HTMLS_FOLDER_option, PDFS_FOLDER_option, names_option, threads_option);

            // subcommand like root command1 command2
            /*var command = new Command("test", "test desc");
            command.SetHandler(OnTest);
            rootCommand.AddCommand(command);*/

            var commandLineBuilder = new CommandLineBuilder(rootCommand)
                .UseDefaults();
            var parser = commandLineBuilder.Build();
            return await parser.InvokeAsync(args).ConfigureAwait(false);
        }
        /*private static void OnTest() => Console.WriteLine("Test");*/
        private static void OnHandle(string HTMLS_FOLDER, string PDFS_FOLDER, string names, int threads_value) {
            if (string.IsNullOrEmpty(HTMLS_FOLDER))
            {
                throw new ArgumentNullException(
                    "--html-folder is required; please ensure that a value is provided");
            }
            
            if (string.IsNullOrEmpty(PDFS_FOLDER))
            {
                throw new ArgumentNullException(
                    "--pdf-folder is required; please ensure that a value is provided");
            }
            if (string.IsNullOrEmpty(names))
            {
                throw new ArgumentNullException(
                    "--names is required; please ensure that a value is provided");
            }

            Converter(HTMLS_FOLDER, PDFS_FOLDER, ParseString(names), threads_value);
        }
    }
}
