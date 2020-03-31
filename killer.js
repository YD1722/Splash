const spawn = require('child_process').spawn;
const exec = require('child_process').exec;
const execFile = require('child_process').execFile;

const arg = process.argv.slice(2);

const params = {
    restart: 'killStart',
    killer: 'kill',
    killService: 'killService',
    killFin: 'killFin'
};

/**
 * Store all the fin process Id s temporary here.
 * @type {Array}
 */
let finIds = [];

if (arg.length >= 2) {
    let param = arg[0];
    let port = arg[1];

    switch (param) {
        case params.restart:
            getProcessesByPortNumber(port).then(processDetails => {
                let processIds = getAllProcessIds(processDetails);

                if (processIds && processIds.length) {
                    killMultipleProcesses(processIds).then(() => {
                        // Restart the app after killing all the instances
                        _launchApp()
                    }).catch(() => {
                        _launchApp();
                    });
                } else {
                    _launchApp();
                }
            });

            break;

        case params.killer:
            getProcessesByPortNumber(port).then(processDetails => {
                let processIds = getAllProcessIds(processDetails);

                if (processIds && processIds.length) {
                    killMultipleProcesses(processIds);
                }
            });

            break;


        case params.killService:
            getProcessesByPortNumber(port).then(processDetails => {
                let processIds = getAllProcessIds(processDetails);

                getProcessIdsByProcessName(processIds, 'runner.exe').then(() => {
                    if (finIds && finIds.length) {
                        killMultipleProcesses(finIds);
                    }
                });
            });

            break;

        default:
            console.log('Invalid parameter');
    }
}


/**
 * Get all the processes running on a given port number
 * @param port
 * @returns {Promise<any>}
 */
function getProcessesByPortNumber(port) {
    return new Promise((res, rej) => {
        let output;

        const pidFinder = spawn('cmd.exe', ['/c', `netstat -ano| findstr :${port}`]);
        let stderrData = new Buffer(0);

        pidFinder.stdout.on('data', (data) => {
            output = data.toString().match(/\S+/g);
        });

        pidFinder.stderr.on('data', err => {
            stderrData = Buffer.concat([stderrData, data]);
        });

        pidFinder.on('exit', (code) => {
            if (stderrData.length) {
                rej();
            } else {
                if (output) {
                    res(output);
                }
            }

            console.log(`process finder exit with code ${code}`);
        });
    });
}

/**
 * Get Process ID's from the process details
 * @param outputStream
 * @returns {*}
 * @private
 */
function getAllProcessIds(processDetails) {
    let processIdList = processDetails.filter(function (value, index) {
        return (index + 1) % 5 === 0;
    });

    return [...new Set(processIdList)];
}

/**
 * Identify whether the pid is from openfin.exe or not
 * @param pid
 * @returns {Promise<any>}
 */
function isGivenProcessName(pid, pName) {
    return new Promise((resolve, reject) => {
        exec(`tasklist | findstr /c:"${pid}"`, (err, data) => {
            if (err) {
                console.log(err);
                reject(err);
            }

            let a = data.toString().match(/\S+/g);

            if (a && a.length) {
                let processName = a[0];

                if (processName === pName) {
                    finIds.push(pid);

                    resolve();
                } else {
                    resolve();
                }
            } else {
                resolve();
            }
        });
    });
}

/**
 * Get all the process id s for a given process name from a given process Id's/. tried this with Promise.all
 * Not working due to parallel execution of Promise.all
 * @param processIds
 */

function getProcessIdsByProcessName(pids, pName) {
    let sequence = Promise.resolve();

    pids.forEach(id => {
        sequence =
            sequence
                .then(() => isGivenProcessName(id, pName))
                .catch();
    });

    return sequence;
}

function killByProcessId(processId) {
    return new Promise((res) => {
        if (processId) {
            /**
             * PID 0 is the System Idle Process. Since that process
             * isn't really a process and never exits
             */
            if (processId === "0") {
                res();
            } else {
                let killer = spawn('cmd.exe', ['/c', `taskkill /PID ${processId} /F`]);

                killer.stderr.on('data', (data) => {
                    console.error('cmd process killer error: ' + data.toString());
                    return res();
                });

                killer.on('exit', (code) => {
                    console.log(`cmd process killer exit with code ${code}`);
                    return res();
                });
            }
        } else {
            res();
        }
    })
}

function killMultipleProcesses(pidList) {
    let sequence = Promise.resolve();

    pidList.forEach(id => {
        sequence =
            sequence
                .then(() => killByProcessId(id));
    });

    return sequence;
}

function _launchApp() {
    execFile('node.exe', ['launcher'], (err) => {
        if (err) {
            console.log(err);
        }
    });
}










