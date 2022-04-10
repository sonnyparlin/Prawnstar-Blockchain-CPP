import React, {useState} from 'react';
import { BrowserRouter } from 'react-router-dom';
import NodeInfo from './NodeInfo';
import News from './News';
import Transactions from './Transactions';
import Wallet from './Wallet';
import BuySellTransfer from './BuySellTransfer';
import Console from './Console';
import Stake from './Stake';

export default function Prawnstar(props) {
    // const [amount, setAmount] = useState("");
    const [address, setAddress] = useState("");
    const setWallet = (amount, address) => {
        setAddress(address);
    }

    // const {path} = props;
    //console.log(props);
    return (
        <BrowserRouter>
            <div className="container-lg">
                <div className="col-4" style={{paddingRight: "10px"}}>
                    <div className="p-2 border bg-white info">
                        <NodeInfo />
                    </div>

                    <div className="p-2 border bg-white news">
                        <b>Crypto News</b><br/>
                        <News />
                    </div>
                </div>  

                <div className="col-4" style={{paddingRight: "10px"}}>
                    <div className="p-2 border bg-white txBox">
                        <b>Transaction History</b>
                        <div className="txhistory">
                        <Transactions />
                        </div>
                    </div>

                    <div className="p-2 border bg-white buysell">
                        <b>Buy / Sell</b>
                        <BuySellTransfer address={address} />
                    </div>
                </div>        

                <div className="col-4" style={{paddingRight: "0px"}}>
                    <div className="p-2 border bg-white wallet">
                        <b>Wallet</b>
                        <Wallet setWallet={setWallet} />
                    </div>
                    <div className="p-2 border bg-white stake">
                        <b>Stake</b>
                        <Stake />
                    </div>
                    <div className="p-2 border bg-white consoleBox">
                        <b>Node Console</b>
                        <div className="console">
                        <Console />
                        </div>
                    </div>
                </div>

            </div>                    
        </BrowserRouter>
    );
}