import React, { useState } from 'react';
import Wallet from './Wallet';

const BuySellTransfer = (props) => {

    const [hidden, setHidden] = useState(false);
    const [hidden1, setHidden1] = useState(false);
    const [hidden2, setHidden2] = useState(false);
    const [hidden3, setHidden3] = useState(false);
    const [hiddenSell, setHiddenSell] = useState(false);
    const [hiddenBuy, setHiddenBuy] = useState(false);
    const [hiddenCancelBuy, setHiddenCancelBuy] = useState(false);
    const [hiddenCancelSell, setHiddenCancelSell] = useState(false);
    const [hiddenTransfer, setHiddenTransfer] = useState(false);
    const [hiddenCancelTransfer, setHiddenCancelTransfer] = useState(false);
    const [hiddenStake, setHiddenStake] = useState(false);
    const [hiddenCancelStake, setHiddenCancelStake] = useState(false);

    const handleBuyClick = async () => {

        const receiver = props.nodeaddress;
        const amount = document.getElementById('tramount').value;

        console.log("address is " + props.nodeaddress);
        console.log(amount);
        var jsonData = {
            "transaction":
            {
                "sender": "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac",
                "receiver": receiver,
                "amount": parseInt(amount),
                "type": "EXCHANGE"
            }
        }
      
        // Send data to the backend via POST
        const response = await fetch('http://127.0.0.1:8001/transact', {
            method: 'POST', 
            mode: 'cors', 
            body: JSON.stringify(jsonData)
        })

        return response.json();
    }

    const handleStakeClick = async () => {

        const receiver = props.nodeaddress;
        const amount = document.getElementById('stamount').value;

        console.log("address is " + props.nodeaddress);
        console.log(amount);
        var jsonData = {
            "transaction":
            {
                "sender": receiver,
                "receiver": receiver,
                "amount": parseInt(amount),
                "type": "STAKE"
            }
        }
      
        // Send data to the backend via POST
        const response = await fetch('http://127.0.0.1:8001/transact', {
            method: 'POST', 
            mode: 'cors', 
            body: JSON.stringify(jsonData)
        })

        return response.json();
    }
  
    return (
    <div>
        <div className="transactionSpace">
        
            <div className="buyForm" hidden={!hiddenBuy}>
                <div className="fleft">
                <input type="text" className="form-control-plaintext" id="tramount" placeholder="amount" />
                </div>
            <button type="submit" className="btn btn-primary mb-2" onClick={() => 
                {
                    handleBuyClick();
                    window.location.reload(false);
                }
            }>
                Buy
            </button>
            </div>

            <div className="sellForm" hidden={!hiddenSell}>
                <div className="fleft">
                <input type="text" className="form-control-plaintext" id="amount" placeholder="amount" />
                </div>
            <button type="submit" className="btn btn-primary mb-2">Sell</button>
            </div>

            <div className="transferForm" hidden={!hiddenTransfer}>
                <div className="fleft">
                <input type="text" className="form-control-plaintext fleft" id="address" placeholder="address" />
                <input type="text" className="form-control-plaintext" id="amount" placeholder="amount" />
                </div>
            <button type="submit" className="btn btn-primary mb-2">Transfer</button>
            </div>

            <div className="stakeForm" hidden={!hiddenStake}>
                <div className="fleft">
                <input type="text" className="form-control-plaintext" id="stamount" placeholder="amount" />
                </div>
            <button type="submit" className="btn btn-primary mb-2" onClick={() => 
                {
                    handleStakeClick();
                    window.location.reload(false);
                }
            }>
                Stake</button>
            </div>

        </div>
        <table>
        <tbody>
        <tr>
        <td>
        <button className="btn btn-primary buy" hidden={hidden} onClick={() => {
            setHidden(!hidden); 
            setHidden1(!hidden1);
            setHidden2(!hidden2);
            setHidden3(!hidden3);
            setHiddenBuy(!hiddenBuy);
            setHiddenCancelBuy(!hiddenCancelBuy);
        }}>
            Buy
        </button>
        </td>
        <td>
        <button className="btn btn-primary" hidden={hidden1} onClick={() => {
            setHidden(!hidden); 
            setHidden1(!hidden1);
            setHidden2(!hidden2);
            setHidden3(!hidden3);
            setHiddenSell(!hiddenSell);
            setHiddenCancelSell(!hiddenCancelSell);
        }}>
            Sell
        </button>
        </td>
        <td>
        <button className="btn btn-primary" hidden={hidden2} onClick={() => {
            setHidden(!hidden); 
            setHidden1(!hidden1);
            setHidden2(!hidden2);
            setHidden3(!hidden3);
            setHiddenTransfer(!hiddenTransfer);
            setHiddenCancelTransfer(!hiddenCancelTransfer);
        }}>
            Transfer
        </button>
        </td>
        <td width="240px" align="right">
        <button className="btn btn-primary" hidden={hidden3} onClick={() => {
            setHidden(!hidden); 
            setHidden1(!hidden1);
            setHidden2(!hidden2);
            setHidden3(!hidden3);
            setHiddenStake(!hiddenStake);
            setHiddenCancelStake(!hiddenCancelStake);
        }}>
            Stake
        </button>

        </td>
        </tr>
        </tbody>
        </table>


        <button className="btn btn-primary fright" hidden={!hiddenCancelBuy} onClick={() => {
            setHidden(!hidden);
            setHidden1(!hidden1);
            setHidden2(!hidden2);
            setHidden3(!hidden3);
            setHiddenBuy(!hiddenBuy);
            setHiddenCancelBuy(!hiddenCancelBuy);
        }}>
            Cancel
        </button>

        <button className="btn btn-primary fright" hidden={!hiddenCancelSell} onClick={() => {
            setHidden(!hidden);
            setHidden1(!hidden1);
            setHidden2(!hidden2);
            setHidden3(!hidden3);
            setHiddenSell(!hiddenSell);
            setHiddenCancelSell(!hiddenCancelSell);

        }}>
            Cancel
        </button>

        <button className="btn btn-primary fright" hidden={!hiddenCancelTransfer} onClick={() => {
            setHidden(!hidden);
            setHidden1(!hidden1);
            setHidden2(!hidden2);
            setHidden3(!hidden3);
            setHiddenTransfer(!hiddenTransfer);
            setHiddenCancelTransfer(!hiddenCancelTransfer);

        }}>
            Cancel
        </button>

        <button className="btn btn-primary fright" hidden={!hiddenCancelStake} onClick={() => {
            setHidden(!hidden);
            setHidden1(!hidden1);
            setHidden2(!hidden2);
            setHidden3(!hidden3);
            setHiddenStake(!hiddenStake);
            setHiddenCancelStake(!hiddenCancelStake);

        }}>
            Cancel
        </button>


    </div>
    );
  
  }
  
  export default BuySellTransfer;