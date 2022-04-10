import React, {useState, useEffect} from 'react';
import $ from 'jquery';

export default function News() {

    const [news, setNews] = useState([{},{}]);

    useEffect(() => {
        getNews();
    }, []);

    const getNews = () => {

        $.get("https://cointelegraph.com/feed",function(data) {    

            var $xml = $(data);   
            var items = [];

            $xml.find("item").each(function() {
                var $this = $(this);
                items.push({
                    title: $this.find("title").text(),
                    link: $this.find("link").text(),
                    description: $this.find("description").text(),
                    pubDate: $this.find("pubDate").text(),
                });
            });

            setNews(items);

        });
    }

    return (
        <div className="App-feeds">
            <div className="panel-list">
                <small>
                <a target="_new" href={news[0] ? news[0].link : null}>{news[0] ? news[0].title : null}</a><br/><small>{news[0] ? news[0].pubDate : null}</small><br/><br/>

                <a target="_new" href={news[1] ? news[1].link : null}>{news[1] ? news[1].title : null}</a><br/><small>{news[1] ? news[1].pubDate : null}</small><br/><br/>

                <a target="_new" href={news[2] ? news[2].link : null}>{news[2] ? news[2].title : null}</a><br/><small>{news[2] ? news[2].pubDate : null}</small><br/><br/>

                <a target="_new" href={news[3] ? news[3].link : null}>{news[3] ? news[3].title : null}</a><br/><small>{news[3] ? news[3].pubDate : null}</small><br/><br/>

                <a target="_new" href={news[4] ? news[4].link : null}>{news[4] ? news[4].title : null}</a><br/><small>{news[4] ? news[4].pubDate : null}</small><br/><br/>

                <a target="_new" href={news[5] ? news[5].link : null}>{news[5] ? news[5].title : null}</a><br/><small>{news[5] ? news[5].pubDate : null}</small><br/><br/>

                <a target="_new" href={news[6] ? news[6].link : null}>{news[6] ? news[6].title : null}</a><br/><small>{news[6] ? news[6].pubDate : null}</small><br/><br/>

                <a target="_new" href={news[7] ? news[7].link : null}>{news[7] ? news[7].title : null}</a><br/><small>{news[7] ? news[7].pubDate : null}</small><br/><br/>

                <a target="_new" href={news[8] ? news[8].link : null}>{news[8] ? news[8].title : null}</a><br/><small>{news[8] ? news[8].pubDate : null}</small><br/><br/>

                <a target="_new" href={news[9] ? news[9].link : null}>{news[9] ? news[9].title : null}</a><br/><small>{news[9] ? news[9].pubDate : null}</small><br/><br/>
                </small>
            </div>
        </div>
    );
}