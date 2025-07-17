import { Card, CardContent } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Badge } from "@/components/ui/badge";
import { Code, Rocket, CheckCircle, FolderOpen, PlusCircle, Play, Bolt, Box, Shield, Book } from "lucide-react";

export default function Home() {
  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-50 to-slate-100">
      {/* Header */}
      <header className="bg-white shadow-sm border-b border-slate-200">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
          <div className="flex justify-between items-center h-16">
            <div className="flex items-center space-x-3">
              <div className="w-8 h-8 bg-blue-600 rounded-lg flex items-center justify-center">
                <Code className="text-white" size={16} />
              </div>
              <div>
                <h1 className="text-lg font-semibold text-slate-900">Project Starter</h1>
                <p className="text-xs text-slate-500">Ready for Development</p>
              </div>
            </div>
            <div className="flex items-center space-x-2">
              <div className="flex items-center space-x-1 text-emerald-600">
                <div className="w-2 h-2 bg-emerald-500 rounded-full animate-pulse"></div>
                <span className="text-sm font-medium">Ready</span>
              </div>
            </div>
          </div>
        </div>
      </header>

      {/* Main Content */}
      <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-12">
        {/* Welcome Section */}
        <Card className="p-8 mb-8">
          <CardContent className="p-0">
            <div className="text-center">
              <div className="w-16 h-16 bg-blue-100 rounded-full flex items-center justify-center mx-auto mb-4">
                <Rocket className="text-blue-600" size={32} />
              </div>
              <h2 className="text-3xl font-bold text-slate-900 mb-2">Welcome to Your New Project</h2>
              <p className="text-lg text-slate-600 mb-6">Your project structure is ready. Let's build something amazing together!</p>
              
              {/* Status Grid */}
              <div className="grid grid-cols-1 md:grid-cols-3 gap-4 mb-8">
                <div className="bg-emerald-50 border border-emerald-200 rounded-lg p-4">
                  <div className="flex items-center justify-center mb-2">
                    <CheckCircle className="text-emerald-600" size={24} />
                  </div>
                  <h3 className="font-semibold text-emerald-900">Project Initialized</h3>
                  <p className="text-sm text-emerald-700">Basic structure is ready</p>
                </div>
                <div className="bg-blue-50 border border-blue-200 rounded-lg p-4">
                  <div className="flex items-center justify-center mb-2">
                    <FolderOpen className="text-blue-600" size={24} />
                  </div>
                  <h3 className="font-semibold text-blue-900">Clean Structure</h3>
                  <p className="text-sm text-blue-700">Organized for expansion</p>
                </div>
                <div className="bg-amber-50 border border-amber-200 rounded-lg p-4">
                  <div className="flex items-center justify-center mb-2">
                    <PlusCircle className="text-amber-600" size={24} />
                  </div>
                  <h3 className="font-semibold text-amber-900">Ready for Packages</h3>
                  <p className="text-sm text-amber-700">Easy to extend and modify</p>
                </div>
              </div>
            </div>
          </CardContent>
        </Card>

        {/* Quick Start Section */}
        <Card className="p-8 mb-8">
          <CardContent className="p-0">
            <h3 className="text-2xl font-bold text-slate-900 mb-6 flex items-center">
              <Play className="text-blue-600 mr-3" size={24} />
              Quick Start Guide
            </h3>
            
            <div className="grid grid-cols-1 lg:grid-cols-2 gap-8">
              {/* Next Steps */}
              <div>
                <h4 className="text-lg font-semibold text-slate-900 mb-4">Next Steps</h4>
                <div className="space-y-3">
                  <div className="flex items-start space-x-3">
                    <div className="w-6 h-6 bg-blue-100 rounded-full flex items-center justify-center flex-shrink-0 mt-0.5">
                      <span className="text-blue-600 text-sm font-semibold">1</span>
                    </div>
                    <div>
                      <p className="font-medium text-slate-900">Install packages</p>
                      <p className="text-sm text-slate-600">Add your required dependencies and tools</p>
                    </div>
                  </div>
                  <div className="flex items-start space-x-3">
                    <div className="w-6 h-6 bg-blue-100 rounded-full flex items-center justify-center flex-shrink-0 mt-0.5">
                      <span className="text-blue-600 text-sm font-semibold">2</span>
                    </div>
                    <div>
                      <p className="font-medium text-slate-900">Configure your environment</p>
                      <p className="text-sm text-slate-600">Set up your development environment</p>
                    </div>
                  </div>
                  <div className="flex items-start space-x-3">
                    <div className="w-6 h-6 bg-blue-100 rounded-full flex items-center justify-center flex-shrink-0 mt-0.5">
                      <span className="text-blue-600 text-sm font-semibold">3</span>
                    </div>
                    <div>
                      <p className="font-medium text-slate-900">Start building</p>
                      <p className="text-sm text-slate-600">Begin developing your application</p>
                    </div>
                  </div>
                </div>
              </div>
              
              {/* Project Structure */}
              <div>
                <h4 className="text-lg font-semibold text-slate-900 mb-4">Project Structure</h4>
                <div className="bg-slate-50 rounded-lg p-4 font-mono text-sm">
                  <div className="space-y-1 text-slate-700">
                    <div className="flex items-center">
                      <FolderOpen className="text-amber-500 mr-2" size={16} />
                      <span>project-root/</span>
                    </div>
                    <div className="flex items-center ml-4">
                      <div className="w-4 h-4 bg-blue-500 rounded-sm mr-2"></div>
                      <span>client/</span>
                    </div>
                    <div className="flex items-center ml-4">
                      <div className="w-4 h-4 bg-green-500 rounded-sm mr-2"></div>
                      <span>server/</span>
                    </div>
                    <div className="flex items-center ml-4">
                      <div className="w-4 h-4 bg-purple-500 rounded-sm mr-2"></div>
                      <span>shared/</span>
                    </div>
                    <div className="flex items-center ml-4">
                      <div className="w-4 h-4 bg-orange-500 rounded-sm mr-2"></div>
                      <span>package.json</span>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </CardContent>
        </Card>

        {/* Development Bolt Section */}
        <Card className="p-8">
          <CardContent className="p-0">
            <h3 className="text-2xl font-bold text-slate-900 mb-6 flex items-center">
              <Bolt className="text-blue-600 mr-3" size={24} />
              Development Ready
            </h3>
            
            <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
              {/* Package Management */}
              <div className="bg-gradient-to-br from-blue-50 to-blue-100 rounded-lg p-6">
                <div className="flex items-center justify-between mb-4">
                  <h4 className="font-semibold text-blue-900">Package Management</h4>
                  <Box className="text-blue-600" size={20} />
                </div>
                <p className="text-sm text-blue-700 mb-4">Ready for npm, yarn, or pnpm package installation</p>
                <div className="bg-white rounded px-3 py-2 text-xs font-mono text-slate-700">
                  npm install package-name
                </div>
              </div>
              
              {/* Error Handling */}
              <div className="bg-gradient-to-br from-emerald-50 to-emerald-100 rounded-lg p-6">
                <div className="flex items-center justify-between mb-4">
                  <h4 className="font-semibold text-emerald-900">Error Handling</h4>
                  <Shield className="text-emerald-600" size={20} />
                </div>
                <p className="text-sm text-emerald-700 mb-4">Basic error handling structure in place</p>
                <div className="bg-white rounded px-3 py-2 text-xs font-mono text-slate-700">
                  try/catch ready
                </div>
              </div>
              
              {/* Documentation */}
              <div className="bg-gradient-to-br from-purple-50 to-purple-100 rounded-lg p-6">
                <div className="flex items-center justify-between mb-4">
                  <h4 className="font-semibold text-purple-900">Documentation</h4>
                  <Book className="text-purple-600" size={20} />
                </div>
                <p className="text-sm text-purple-700 mb-4">Clear documentation for easy onboarding</p>
                <div className="bg-white rounded px-3 py-2 text-xs font-mono text-slate-700">
                  README.md included
                </div>
              </div>
            </div>
            
            {/* Action Buttons */}
            <div className="mt-8 flex flex-col sm:flex-row gap-4 justify-center">
              <Button className="bg-blue-600 hover:bg-blue-700 text-white px-6 py-3" size="lg">
                <Code className="mr-2" size={16} />
                Start Development
              </Button>
              <Button variant="secondary" className="bg-slate-600 hover:bg-slate-700 text-white px-6 py-3" size="lg">
                <Book className="mr-2" size={16} />
                View Documentation
              </Button>
              <Button variant="outline" className="border-slate-300 hover:border-slate-400 text-slate-700 px-6 py-3" size="lg">
                <PlusCircle className="mr-2" size={16} />
                Add Package
              </Button>
            </div>
          </CardContent>
        </Card>
      </main>
      
      {/* Footer */}
      <footer className="bg-white border-t border-slate-200 mt-16">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
          <div className="text-center">
            <p className="text-sm text-slate-500">
              Project initialized and ready for development • 
              <span className="text-emerald-600 font-medium">Status: Ready</span>
            </p>
            <p className="text-xs text-slate-400 mt-2">
              Built with modern web standards • Designed for scalability
            </p>
          </div>
        </div>
      </footer>
    </div>
  );
}
