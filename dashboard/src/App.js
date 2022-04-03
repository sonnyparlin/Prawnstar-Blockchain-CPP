import './App.css';
import React from 'react';
import News from './News.js';
import Console from './Console.js';
import Wallet from './Wallet.js';
import Stake from './Stake.js';
import Transactions from './Transactions.js';
import BuySellTransfer from './BuySellTransfer';

class App extends React.Component {

    constructor(props) { 
        super(props);
        this.state = { nodeid: "", ip: "", port: "", master: "" };
    }

    componentDidMount() {
        this.getNodeID();
    }

    getNodeID() {

        fetch("http://127.0.0.1:8001/node")
            .then(response => response.json())
            .then((jsonData) => {
                // jsonData is parsed json object received from url
                this.setState({ nodeid: jsonData.message });
                this.setState({ ip: jsonData.ip });
                this.setState({ port: jsonData.port });
                this.setState({ master: jsonData.master });
            })
            .catch((error) => {
                // handle your errors here
                console.error(error);
            });
    }

    render() { 
        return (
            <div className="container-lg">
                <div className="col-4">
                    <div className="p-2 border bg-white info">
                        <b>Node: {this.state.nodeid}</b><br/>
                        Node IP: {this.state.ip}<br/>
                        Node Port: {this.state.port}<br/>
                        Master Node: {this.state.master}<br/><br/>
                        <button className="btn btn-primary">Configure Node</button>
                
                    </div>

                    <div className="p-2 border bg-white news">
                        <b>Crypto News:</b><br/>
                        <News/>
                    </div>
                </div>  

                <div className="col-4">
                    <div className="p-2 border bg-white consoleBox">
                        <b>Node Console</b>
                        <div className="console">
                            <Console/>
                        </div>
                    </div>

                    <div className="p-2 border bg-white buysell">
                        <b>Buy / Sell</b>
                        <BuySellTransfer/>
                    </div>
                </div>        

                <div className="col-4">
                    <div className="p-2 border bg-white wallet">
                        <b>Wallet</b>
                        <Wallet/>
                    </div>
                    <div className="p-2 border bg-white stake">
                        <b>Stake</b>
                        <Stake/>
                    </div>
                    <div className="p-2 border bg-white txhistory">
                        <b>Transaction History</b>
                        <Transactions/>
                    </div>
                </div>

            </div>
        );
    }
}

export default App;
