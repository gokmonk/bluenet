
desc "Compiling debug version"
task :debug do

end

desc "Running unit tests"
task :test do
	sh "echo $BLUENET_BUILD_DIR" 
	sh "cd $BLUENET_BUILD_DIR/test && make test"
end

desc "Compiling debug version and running tests"
task :default => [:debug, :test] do
end

