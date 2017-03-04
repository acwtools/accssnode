/*
 * accssnode (C) 2016 by Gandalf Sievers
 * Created by Gandalf Sievers on 26.05.16.
 *
 * MIT-License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

"use strict";

var compressor = require('../build/Release/accssnode'),
    process = require('process'),
    fs = require('fs');


fs.readdir('./accss/tests/css/src', function(err, files){
    if (err) throw err;
    files.forEach(function(file){
        fs.readFile('./accss/tests/css/src/' + file, 'utf8', (err, data) => {
          if (err) throw err;
          var res = compressor.minify(data, {"restructure" : true, 'preserve-splitted' : false});
          fs.readFile('./accss/tests/css/res/' + file.replace(/\.css$/i, ".res"), 'utf8', (err, dataResult) => {
            if (err) throw err;
            if(res.trim() != dataResult.trim()) {
                console.log(file + ' \x1b[1m\x1b[31mFailed!\x1b[0m');
                console.log(dataResult);
                console.log('========================');
                console.log(res);
                process.exitCode = 1;
            } else {
                console.log(file + ' \x1b[1m\x1b[32mPassed!\x1b[0m');
            }
          });
        });
    });
});


